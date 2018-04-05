#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;
int main(int argc, char const *argv[])
{
	/* code */
	ifstream ifile;
	ifile.open("Vertices.txt");
	int a,b,c=0;
	for(int i=0;i<5;i++)
	{
		std::string line;
		std::getline(ifile,line);
		std::istringstream iss(line);
		
		if(!(iss>>a>>b>>c))
		{
			cout<<a<<" "<<b<<endl;
		}
		else
		{
			cout<<a<<" "<<b<<" "<<c<<endl;
		}
		

	}
	return 0;
}