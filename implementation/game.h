#ifndef GAME_H_
#define GAME_H_

#include "hextypes.h"
#include "mcts_tree.h"

namespace Hex {

class Game {
public:
	Game();
	void ClearBoard();
	void Play(const Move& move);
	Move GenMove(Player player);
	void SetMaxUTCTreeDepth(uint depth);
	void PrintBoard(std::string& board);
	void SetPerMove(uint playouts);
	void PrintTree(std::string& ascii_tree, uint children);
	bool IsValidMove(const Move& move);
	bool IsFinished();
	void setDefendingBridges(bool v);
	void setAvoidingBridges(bool v);
    Player CurrentPlayer();
    Player nowWinner();

private:
	Board current_board;
	Board empty_board;
	Location last_move;
	MCTSTree tree;
};

} // namespace Hex

#endif /* GAME_H_ */
