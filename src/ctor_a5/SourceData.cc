#include "SourceData.h"
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include "Board.h"
#include "Tile.h"

using namespace std;

SourceData::SourceData(bool seed, int sVal): isSave{false}, isSeed{seed} {
  if (isSeed){
    seedVal = sVal;
    srand(seedVal);
  }
  else {
    seedVal = -1;
    srand(seedVal);
  }
  // int Brd[38];
  int List[19][2];
  int a = 4;
  int b = 4;
  int c = 4;
  int d = 3;
  int e = 3;
  int f = 1;
  string SourceFile = "0";
  for (int i = 0; i < 19; i++){
    List[i][0] = rand()%11 + 2;
    if (a != 0){
      List[i][1] = 0;
      a--;
    }
    else if (b != 0){
      List[i][1] = 1;
      b--;
    }

    else if (c != 0){
      List[i][1] = 2;
      c--;
    }

    else if (d != 0){
      List[i][1] = 3;
      d--;
    }
    else if (e != 0){
      List[i][1] = 4;
      e--;
    }
    else if (f != 0){
      List[i][1] = 5;
      f--;
    }
  }
  int i, j;
  for (i = 0; i < 19; i++){
       for (j = 0; j < 19-i-1; j++){
           if (List[j][0] > List[j+1][0]){
            using std::swap;
                swap(List[j][1], List[j+1][1]);
                swap(List[j][0], List[j+1][0]);
           }
        }
   }
   for (int i = 0; i < 19; i++){
     Brd[i] = List[i][0];
   }
  for (int i = 19; i < 38; i++){
    Brd[i] = rand()%11+2;
  }
}
    
// For new game with default board
SourceData::SourceData(string SourceFile):  isSave{false}, isSeed{false}, seedVal{-1}, SourceFile{SourceFile} {
  // int Brd[38]
  ifstream File;
  File.open(SourceFile);
  for (int i = 0; i < 19; i++){
    File >> Brd[i];
    // cout << Brd[i];
    File >> Brd[i+19];

    // cout << Brd[i+19];
  }
}

// For new game with default board
SourceData::SourceData(bool saved, string File):
  isSave{saved}, isSeed{false}, seedVal{-1}, SourceFile{File} {
  stringstream in(File);
  for (int i = 0; i < 19; i++){
    // First 19 are for tile resources
    // 20 - 37 are for the corresponding tileVal
    // pairs (index 0, index 19), (index 1, index 20), etc...
    in >> Brd[i];
    in >> Brd[i+19]; 
  }
}

int SourceData::Getdata(int num){
  return Brd[num];
}

// attach Board to sourcedata obj AFTER it has been constructed
void SourceData::attachBoard(shared_ptr<Board> b) {
  PBoard = b;
}

void SourceData::setSaved(bool s)
{
  isSave=s;
}

void SourceData::NotifyBoard(){
  for(int i = 0; i < 19; i++){
    shared_ptr<Tile> PTile = PBoard->getTile(i);
    Resources res[]={Resources::BRICK,Resources::ENERGY,Resources::GLASS,Resources::HEAT,Resources::WIFI,Resources::PARK};
    PTile->setResource(res[Brd[i]]);
    // cout << Brd[i];
    PTile->setTileVal(Brd[i+19]);
    
    if (!isSave && i == 6){
      // cout << "Geese Set" << endl;
      PTile->setGeese(true);
      // cout << PTile->existGeese() << endl;
    }

  }
}
