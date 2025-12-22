#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "board/board.h"
#include "board/gameMaster.h"

using namespace std;

// Función auxiliar para visualizar el tablero rápidamente en consola
void debugPrint(GameMaster& gm, Board& b) {
    cout << b.print() << endl;
    cout << "Turno: " << (gm.getPlayerPlaying() ? "BLANCO (O)" : "ROJO (X)") << endl;
}

// --- TEST 1: LÍMITES DEL TABLERO ---
// Verifica que una pieza en el borde no pueda moverse fuera.
void testBoundaries() {
    cout << "\n[TEST 1] Verificando Limites del Tablero..." << endl;
    Board board;
    GameMaster gm(&board, false); // Juega Rojo

    // Colocamos pieza Roja en (2,7) - Borde Derecho
    Bitboard red = 1ULL << (7 + 2 * 8); // Fila 2, Col 7
    board.setAllBoards({0, red, 0}, 0, 1, 0, 0);

    debugPrint(gm, board);

    vector<Move> moves = gm.getPossibleMoves();
    bool boundaryError = false;

    for (auto& m : moves) {
        // Si intenta moverse a la DERECHA desde la columna 7, es un error
        if (m.rl == RIGHT) {
            cout << "FALLO: Se detecto movimiento ilegal hacia la derecha desde el borde: " << m.str() << endl;
            boundaryError = true;
        }
    }

    if (!boundaryError) {
        cout << "PASO: La pieza en el borde derecho no genero movimientos fuera del mapa." << endl;
    } else {
        cout << "FALLO: Revisar Board::canMove (el check de newPos == 0)." << endl;
    }
}

// --- TEST 2: SALTO OBLIGATORIO ---
// Si hay captura disponible, los movimientos normales deben estar prohibidos.
void testForcedJump() {
    cout << "\n[TEST 2] Verificando Captura Obligatoria..." << endl;
    Board board;
    GameMaster gm(&board, false); // Juega Rojo

    // Escenario:
    // Roja en (4,4) puede comer a Blanca en (5,5).
    // Roja en (1,0) podria moverse normalmente si no fuera obligatorio comer.
    Bitboard red = (1ULL << (4 + 4 * 8)) | (1ULL << (0 + 1 * 8));
    Bitboard white = (1ULL << (5 + 5 * 8));
    board.setAllBoards({white, red, 0}, 1, 2, 0, 0);

    debugPrint(gm, board);

    vector<Move> moves = gm.getPossibleMoves();
    bool normalMoveFound = false;
    bool jumpFound = false;

    for (auto& m : moves) {
        cout << "Movimiento Posible: " << m.str() << endl;
        if (m.capturedPos != 0) jumpFound = true;
        else normalMoveFound = true;
    }

    if (jumpFound && !normalMoveFound) {
        cout << "PASO: Solo se permitieron saltos. Movimientos normales bloqueados." << endl;
    } else {
        cout << "FALLO: ";
        if (!jumpFound) cout << "No se detecto el salto posible. ";
        if (normalMoveFound) cout << "Se permitieron movimientos normales habiendo captura. ";
        cout << endl;
    }
}

// --- TEST 3: BLOQUEO EN SALTO MULTIPLE ---
// En medio de una cadena, solo la pieza que salta debe poder moverse.
void testMultiJumpLock() {
    cout << "\n[TEST 3] Verificando Bloqueo en Salto Multiple (Chain-Lock)..." << endl;
    Board board;
    GameMaster gm(&board, false); // Juega Rojo

    Bitboard red = (1ULL << (2 + 2 * 8)) | (1ULL << (6 + 2 * 8));
    Bitboard white = (1ULL << (3 + 3 * 8)) | (1ULL << (5 + 5 * 8));
    board.setAllBoards({white, red, 0}, 2, 2, 0, 0);

    cout << "--- Estado Previo al Primer Salto ---" << endl;
    debugPrint(gm, board);

    // Ejecutar primer salto: (2,2) come a (3,3) y cae en (4,4)
    cout << ">>> Ejecutando salto (2,2) -> (4,4)..." << endl;
    bool moveSuccess = gm.movePiece(2, 2, RIGHT, DOWN);

    if (!moveSuccess) {
        cout << "ERROR FATAL: El primer salto no fue valido. Revisa la logica basica." << endl;
        return;
    }

    cout << "--- Estado 'En el aire' (Esperando segundo salto) ---" << endl;
    debugPrint(gm, board);

    // Ahora pedimos los movimientos posibles.
    // DEBERIA haber solo 1: El salto de (4,4) a (6,6).
    // NO DEBERIA aparecer la pieza distractora (2,6).
    vector<Move> moves = gm.getPossibleMoves();
    bool distractoraMueve = false;
    bool cadenaCorrecta = false;

    for (auto& m : moves) {
        int r = __builtin_ctzll(m.pos) / 8;
        int c = __builtin_ctzll(m.pos) % 8;
        
        cout << "Movimiento Posible: " << m.str() << endl;

        if (r == 2 && c == 6) distractoraMueve = true;
        if (r == 4 && c == 4) cadenaCorrecta = true;
    }

    if (cadenaCorrecta && !distractoraMueve) {
        cout << "PASO: Sistema bloqueo correctamente las otras piezas durante la cadena." << endl;
    } else {
        cout << "FALLO: ";
        if (distractoraMueve) cout << "Se permitio mover otra pieza (2,6) durante la cadena. ";
        if (!cadenaCorrecta) cout << "No se ofrecio el segundo salto obligatorio. ";
        cout << endl;
    }
}

int main() {
    cout << "=== INICIANDO SUITE DE PRUEBAS ===" << endl;
    
    testBoundaries();
    testForcedJump();
    testMultiJumpLock();

    cout << "\n=== PRUEBAS FINALIZADAS ===" << endl;
    return 0;
}