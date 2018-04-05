#include "Board.h"
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){
    string first, second;
    if (argc == 3){
        first = argv[1];
        second = argv[2];
    }
    bool firsttime = false;
    int a;
    shared_ptr<Board> b;
    if (first == "-seed" || first == "-board"){
        firsttime = true;
    }
    if (first == "-seed"){
      istringstream s{second};
      s >> a; // pass in seed int value
      cout << "a: " << a << endl;
      // Construct then attach...
      b = make_shared<Board>(true, a, "BoardSkeleton.txt");
      b->attachBoard(b);
      b->updateBoard();
      // ------------------------
    }
    else {
    // Construct then attach...
        if (first == "-board"){
            b = make_shared<Board>(false, false,-1, second, "BoardSkeleton.txt");
        }
        else{ 

            b = make_shared<Board>(true, false,-1, second, "BoardSkeleton.txt");
            // cout << "file to read: Construct" << second << endl;
        }
        b->attachBoard(b);
        b->updateBoard();
    // ------------------------
    }
    if (firsttime){
        b->chosestarting();
    }
    while(true){
        for (int i = 0; i < 4; i++){
             b->playturn(i);
        }
        for (int i = 0; i < 4; i++){
            // auto a = make_shared<Player>();
            shared_ptr<Player> a = b->getPlayer(i);
            //Comparing the player points to end game if someone wins
            if (a->playerStatuses() > 9){
                string a;
                cout << "Would you like to play again?" << endl;
                cout << "> ";
                cin >> a;
                if (a == "yes"){
                    if (firsttime){
                        if (first == "-seed"){
                            stringstream s(second);
                            int a;
                            s >> a;

                            // Construct then attach...
                            b = make_shared<Board>(true, a, "BoardSkeleton.txt");
                            b->chosestarting();
                        }
                        else{

                        	// Construct then attach...
                            b.reset();
                            b = (make_shared<Board>(true, false,-1, second, "BoardSkeleton.txt"));
                            b->attachBoard(b);
                            b->updateBoard();

                            b->chosestarting();
                        }
                    }
                    else{

                    	// Construct then attach...
                        b.reset();
                        b = (make_shared<Board>(true, -1, "BoardSkeleton.txt"));
                        b->attachBoard(b);
                        b->updateBoard();

                        b->chosestarting();
                    }
                }
                else{
                    return 0;
                }
            }
        }
                            
    }
}
