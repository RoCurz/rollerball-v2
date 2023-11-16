#include <algorithm>
#include <random>
#include <iostream>
#include <climits>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"

int duration;
U16 last_move;

int evaluation(const Board& board){
    // std::string board_string = board_to_str(board.data.board_0);
    auto* b_data = &board.data;

    // int n = board_string.size();
    int w_p = 0, w_r = 0, w_b = 0, w_k = 0 , b_k = 0 ,b_p = 0, b_r = 0, b_b = 0;
    // std::vector<U8> pieces = {board.data.b_rook_ws,board.data.b_rook_bs,board.data.b_bishop,board.data.b_pawn_bs,board.data.b_rook_ws,
    //                         board.data.w_rook_ws,board.data.w_rook_bs,board.data.w_bishop,board.data.w_pawn_bs,board.data.w_rook_ws};

    // for (int i = 0; i < 12; i++)
    // {   
    //     if (pieces[i]==DEAD){
    //             continue;
    //         }
    //     if ((board.data.board_0[pieces[i]] & (BLACK|WHITE)) == WHITE){
    //         if ((board.data.board_0[pieces[i]] & ROOK) == ROOK)
    //         {
    //             w_r+=1;
    //         }
    //         else if ((board.data.board_0[pieces[i]] & BISHOP) == BISHOP)
    //         {
    //             w_b+=1;
    //         }
    //         else if ((board.data.board_0[pieces[i]] & PAWN) == PAWN)
    //         {
    //             w_p+=1;
    //         }
    //     }
    //     else
    //     {
    //         if ((board.data.board_0[pieces[i]] & ROOK) == ROOK)
    //         {
    //             b_r+=1;
    //         }
    //         else if ((board.data.board_0[pieces[i]] & BISHOP) == BISHOP)
    //         {
    //             b_b+=1;
    //         }
    //         else if ((board.data.board_0[pieces[i]] & PAWN) == PAWN)
    //         {
    //             b_p+=1;
    //         }
    //     }  
    // }
    if (b_data->b_rook_1   != DEAD) b_r+=1;
    if (b_data->b_rook_2   != DEAD) b_r+=1;
    // if (b_data->b_king     != DEAD) b_k+=1;
    if (b_data->b_bishop   != DEAD) b_b+=1;
    if (b_data->b_knight_1 != DEAD) b_k+=1;
    if (b_data->b_knight_2 != DEAD) b_k+=1;
    if (b_data->b_pawn_1   != DEAD) b_p+=1;
    if (b_data->b_pawn_2   != DEAD) b_p+=1;
    if (b_data->b_pawn_3   != DEAD) b_p+=1;
    if (b_data->b_pawn_4   != DEAD) b_p+=1;

    if (b_data->w_rook_1   != DEAD) w_r+=1;
    if (b_data->w_rook_2   != DEAD) w_r+=1;;
    // if (b_data->w_king     != DEAD) w_k+=1;
    if (b_data->w_bishop   != DEAD) w_b+=1;
    if (b_data->w_knight_1 != DEAD) w_k+=1;
    if (b_data->w_knight_2 != DEAD) w_k+=1;
    if (b_data->w_pawn_1   != DEAD) w_p+=1; 
    if (b_data->w_pawn_2   != DEAD) w_p+=1;
    if (b_data->w_pawn_3   != DEAD) w_p+=1;
    if (b_data->w_pawn_4   != DEAD) w_p+=1;
    int ans;
    ans = 1000*(w_r-b_r)+600*(w_b-b_b)+100*(w_p-b_p);
    if (board.data.player_to_play==(PlayerColor)BLACK){
        ans *= -1;
    }
    if (board.in_check()){
        ans -= 300;
    }
    auto move_s = board.get_legal_moves();
    std::vector<U16> moves(move_s.begin(),move_s.end());
    ans += 5*moves.size();
    Board dummyboard(board);
    dummyboard.do_move_(0);
    if (board.in_check()){
        ans += 300;
    }
    auto move_s_dummy = dummyboard.get_legal_moves();
    std::vector<U16> moves_dummy(move_s_dummy.begin(),move_s_dummy.end());
    ans -= 5*moves_dummy.size();
    if (moves_dummy.size()==0){
        ans += 100000;
    }
    // if(board.data.player_to_play==1<<6){
    //     ans = w_p+5*w_r+3*w_b-b_p-5*b_r-3*b_b;
    //     if (board.in_check()){
    //         ans-=10;
    //     }
    //     auto move_s = board.get_legal_moves();
    //     std::vector<U16> moves(move_s.begin(),move_s.end());
    //     ans+=moves.size()/100;
    //     if (moves.size()==0){
    //         ans = -float('inf');
    //     }      
    // }
    // else{
    //     ans = (w_p+5*w_r+3*w_b-b_p-5*b_r-3*b_b);
    //     if (board.in_check()){
    //         ans-=10;
    //     }
    //     auto move_s = board.get_legal_moves();
    //     std::vector<U16> moves(move_s.begin(),move_s.end());
    //     ans+=moves.size()/100;
    //     // if (moves.size()==0){
    //     //     ans = -float('inf');
    //     // }
    // }
    return ans;
}

int minmax(const Board& board, int depth, int alpha, int beta, std::atomic<bool> t) 
    {
        if (depth == 0 || !search) {
            int eval = evaluation(board);
            // std::cout << "value of board" << eval << std::endl;
            return eval;
        }
        auto move_s = board.get_legal_moves();
        std::vector<U16> moves(move_s.begin(),move_s.end());
        if (depth%2 == 0) {
            int maxEval = INT_MIN;
            for (size_t i = 0; i < moves.size(); i++) {
                Board b(board);  // Create a copy of the board
                b.do_move_(moves[i]);
                int ans = minmax(b, depth - 1, alpha, beta,search);
                maxEval = std::max(ans, maxEval);
                alpha = std::max(alpha, ans);
                if (beta <= alpha) {
                    break; }
      |  ^

                }
            }
            return maxEval;
        } else {
            int minEval = INT_MAX;
            for (size_t i = 0; i < moves.size(); i++) {
                Board b(board);  // Create a copy of the board
                b.do_move_(moves[i]);
                int ans = minmax(b, depth - 1, alpha, beta,search);
                minEval = std::min(ans, minEval);
                beta = std::min(beta, ans);
                if (beta <= alpha) {
                    break;
                }
            }

            return minEval;
        }
    }

void Engine::find_best_move(const Board& b) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto moveset = b.get_legal_moves();
    std::vector<U16> moves(moveset.begin(),moveset.end());
    int opt_move = 0;
    U8 p0 = getp0(last_move);
    U8 p1 = getp1(last_move);
    U16 symmetric_move = p0 | p1<<8;
    
    // if (b.data.player_to_play==(1<<6)){
    std::cout << "last move\t"<< move_to_str(last_move)<<std::endl;
    std::cout << "symmetric move\t"<< move_to_str(symmetric_move)<<std::endl;
    int opt_val = INT_MIN;
    for (int i=0;i<moves.size();i++){
        int x;
        auto b_copy(b);
        if (moves[i] == symmetric_move){
            x = -20;
            std::cout << "value of move\t"<< move_to_str(moves[i])<< "\t" << x << std::endl;
        }
        if (moves.size()>6){
            b_copy.do_move_(moves[i]);
            x = minmax(b_copy,3,INT_MIN,INT_MAX,search);
            std::cout << "value of move\t"<< move_to_str(moves[i])<< "\t" << x << std::endl;
        }
        else
        {
            b_copy.do_move_(moves[i]);
            x = minmax(b_copy,5,INT_MIN,INT_MAX,search);
            std::cout << "value of move\t"<< move_to_str(moves[i])<< "\t" << x << std::endl;
        }
        
        if (x>opt_val){
            opt_val=x;
            opt_move=i;
        }
    }
    // }
    // else{
    //     int opt_val = INT_MAX;
    //     for (int i=0;i<moves.size();i++){
    //         auto b_copy = *b.copy();
    //         b_copy.do_move_(moves[i]);
    //         int x = minmax(b_copy,3,INT_MIN,INT_MAX);
    //         if (x<opt_val){
    //             opt_val=x;
    //             opt_move=i;
    //         }
    //     }
    // }
    this->best_move = moves[opt_move];
    last_move = moves[opt_move];
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Output the elapsed time
    std::cout << "Time taken by function: " << duration.count() << " milliseconds" << std::endl;
    // if (moveset.size() == 0) {
    //     this->best_move = 0;
    // }
    // else {
    //     std::vector<U16> moves;
    //     std::cout << all_boards_to_str(b) << std::endl;
    //     for (auto m : moveset) {
    //         std::cout << move_to_str(m) << " ";
    //     }
    //     std::cout << std::endl;
    //     std::sample(
    //         moveset.begin(),
    //         moveset.end(),
    //         std::back_inserter(moves),
    //         1,
    //         std::mt19937{std::random_device{}()}
    //     );
    //     this->best_move = moves[0];
    // }

}