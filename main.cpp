#include <iostream>
#include <bitset>
#include <fstream>
#include <string>

using namespace std;

#define ONE_NUM_COLS 3
#define ONE_NUM_ROWS 4
#define ACCOUNT_ID_COLS 27

struct AccountNum
{
    
    bitset<9> AccBitMap;
    char AccNum[ONE_NUM_ROWS][ONE_NUM_COLS];
};

struct AccountID
{
    
    char accChars[ONE_NUM_ROWS][ACCOUNT_ID_COLS];
    

};




int main(int argc, char** aargc)
{
	std::cout << "Hello world!\n";

    
   
    
    AccountID accId;
    ifstream ifstr("AccountIdRecords.txt");
    for(int i = 0; i < ONE_NUM_ROWS; i++)
    {
        ifstr.getline(accId.accChars[i], ACCOUNT_ID_COLS);
     
    }
    
    cout << endl;
    for(int i =0; i < ONE_NUM_ROWS; i++)
    {
        for (int j = 0; j < ACCOUNT_ID_COLS; j++)
        {
            printf("%c", accId.accChars[i][j]);
        }
        cout << endl;
    }
    cout << endl;
}










