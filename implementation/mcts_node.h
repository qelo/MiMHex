#ifndef MCTS_NODE_H_
#define MCTS_NODE_H_

#include "hextypes.h"
#include "board.h"
#include "auto_pointer.h"
#include "params.h"

namespace Hex {

/*
 * TODO LIST:
 *
 * (1) Remove ucb_valid flag handling from the main loop inside BestMove().
 *
 * (2) Move as many operations on MCTS nodes from the main loop inside BestMove()
 *     to MCTS node itself as possible.
 *
 * (3) Add Switches to the code.
 *
 * (4) Add PATH AMAF table to the MCTS tree (also to MCTS nodes).
 *
 * (5) Use a single is_valid flag instead of ucb_valid, rave_valid, etc.
 *     Remove separate Compute...() procedures and place the memoization inside
 *     a single Compute() procedure.
 */

class Statistics {
    public:
        Statistics(uint init_won, uint init_played);

        float GetMu();
        float GetBound();
        float GetValue();

        void Won();
        void Lost();

    public:
        uint won;
        uint played;
};

class MCTSNode {
    public:
        MCTSNode();
        MCTSNode(Location loc);

        MCTSNode* SelectBestChild();
        MCTSNode* SelectChild();
        MCTSNode* GetChild(uint index);

        Player GetPlayer();

        void Update(const Board& board, uint* begin, uint* end);

        uint GetPlayed();
        bool IsLeaf();
        void Expand(Board& board);

        void ToAsciiArt(std::ostream& stream, uint max_children, uint max_level);

    private:
        float Compute();

        float GetMu();
        float GetUcbWeight();
        float GetRaveWeight();
        float GetAmafWeight();

        void RecursivePrint(std::ostream& stream, uint max_children, uint max_level, uint level, Player player);

        friend class MCTSTree;

    public:
        Statistics ucb;
        Statistics rave;
        Statistics path;

    private:
        Location loc;
        uint count;
        AutoTable<MCTSNode> children;
        AutoTable<MCTSNode*> pos_to_children_mapping;
        float value;
        bool computed;
};

} // namespace Hex

#endif /* MCTS_NODE_H_ */
