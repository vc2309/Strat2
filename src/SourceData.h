#ifndef _SOURCEDATA_H
#define _SOURCEDATA_H

#include <memory>
#include <vector>
#include <string>

class Board;

class SourceData{
  bool isSave;
	bool isSeed;
	int seedVal;
	std::shared_ptr<Board> PBoard;
	int Brd[38]; // const size
	std::string SourceFile;
public:
 // Always need to pass reference shared ptr whenever constructing a SourceData
 // Allows us mutate the Board/Notify

 // For random generation NEW GAME, pass by ref as we intent to modify
  SourceData(bool seed, int sVal);

  // For provided Board Layout generation NEW GAME
  SourceData(std::string SourceFile);

  SourceData(bool save, std::string File);
  int Getdata(int num);

  // Perform attachment after Board is constructed
  void attachBoard(std::shared_ptr<Board> b);
  void NotifyBoard();
  void setSaved(bool);


};

#endif
