#include "gtest/gtest.h"
#include "base/BaseTypes.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>

using namespace std;

const WORD32 ACCOUNT_DIGIT_ROWS = 3;
const WORD32 ACCOUNT_DIGIT_COLS = 3;
const WORD32 ACCOUNT_DIGIT_BIT_SIZE = 9;


const WORD32 ACCOUNT_ID_DIGIT_NUM = 9;
const WORD32 ACCOUNT_ID_ROWS = ACCOUNT_DIGIT_ROWS;
const WORD32 ACCOUNT_ID_COLS = 27;

const WORD32 DIGIT_TOTAL_NUM = (ACCOUNT_ID_DIGIT_NUM + 1);

const WORD32 DIGIT_INVALID_VALUE = 0xFFFFFFFF;


enum DigitStatus
{
	STATUS_NORMAL = 0,
	STATUS_ILL,
	STATUS_ERROR
};

/*
    _  _     _  _  _  _  _  _
  | _| _||_||_ |_   ||_||_|| |
  ||_  _|  | _||_|  ||_| _||_|

012
345
678

Digit Char and Real Num 's bit map relation:
             012 345 678(Bit)
Real Num:0 = 010 101 111      Value:175
         1 = 000 001 001      Value:9
         2 = 010 011 110      Value:158
         3 = 010 011 011      Value:155
         4 = 000 111 001      Value:57
         5 = 010 110 011      Value:179
         6 = 010 110 111      Value:183
         7 = 010 001 001      Value:137
         8 = 010 111 111      Value:191
         9 = 010 111 011      Value:187
*/


struct AccountDigit
{
	AccountDigit(): realNum(0)
	{
		bitsetChar.reset();
	}

	WORD32 status;
	WORD32 realNum;
	WORD32 bitMapValue;
	bitset<ACCOUNT_DIGIT_BIT_SIZE> bitsetChar;
	set<WORD32> validPosSet;
	char digit[ACCOUNT_DIGIT_ROWS][ACCOUNT_DIGIT_COLS];
};

struct AccountID
{
	AccountID() : curAccntNum(0), validAccntNum(0)
	{
		for (WORD32 idx = 0; idx < ACCOUNT_ID_ROWS; idx++)
		{
			memset(accChars[idx], 0x0, ACCOUNT_ID_COLS);
		}
	}

	void saveAccntDigit(const AccountDigit& accDigit)
	{
		if (curAccntNum >= ACCOUNT_ID_DIGIT_NUM) return;
		accountNum[curAccntNum++] = accDigit;
	}

	void saveAccntCharsWithLine(WORD32 lineIdx, const string& line)
	{
		if (lineIdx >= ACCOUNT_ID_ROWS) return;
		line.copy(accChars[lineIdx], ACCOUNT_ID_COLS);
	}

	void dumpAccntChars()
	{
		cout << "=====begin print Account ID ===== " << endl;
		for(WORD32 i =0; i < ACCOUNT_ID_ROWS; i++)
		{
			for (WORD32 j = 0; j < ACCOUNT_ID_COLS; j++)
			{
				//printf("%c", accntId.accChars[i][j]);
				cout << accChars[i][j];
			}
			cout << endl;
		}
		cout << "=====end print Account ID ===== " << endl;
	}

	bool checkAccntSum()
	{
		WORD32 totalDigitSum = 0;
		for (WORD32 idx = 0; idx < ACCOUNT_ID_DIGIT_NUM; idx++)
		{
			 //累加校验和
			totalDigitSum += (accountNum[idx].realNum * (10 - (idx + 1)));
		}

		cout << "Account ID Check Sum:" << totalDigitSum << endl;
		return totalDigitSum != 0 && totalDigitSum % 11 == 0;
	}

	//need to optimize
	char accChars[ACCOUNT_ID_ROWS][ACCOUNT_ID_COLS];
private:
	WORD32 curAccntNum;
	WORD32 validAccntNum;
	WORD32 illAccntNum;
	AccountDigit accountNum[ACCOUNT_ID_DIGIT_NUM];
};


WORD32 convertDigitCharToNum(WORD32 value)
{
	/*
	Real Num:0 = 010 101 111      Value:175
	         1 = 000 001 001      Value:9
	         2 = 010 011 110      Value:158
	         3 = 010 011 011      Value:155
	         4 = 000 111 001      Value:57
	         5 = 010 110 011      Value:179
	         6 = 010 110 111      Value:183
	         7 = 010 001 001      Value:137
	         8 = 010 111 111      Value:191
	         9 = 010 111 011      Value:187
	*/
	static WORD32 accntIdDigitValue[DIGIT_TOTAL_NUM] = {175, 9, 158, 155, 57, 179, 183, 137, 191, 187};
	for (WORD32 idx = 0; idx < DIGIT_TOTAL_NUM; idx++)
	{
		if (accntIdDigitValue[idx] == value) return idx;
	}

	return DIGIT_INVALID_VALUE;
}


int main(int argc, char* argv[])
{
   testing::InitGoogleTest(&argc, argv);


   AccountID accntId;
   //ifstream ifstr("/home/hbm/MyShares/tlcomp/201711TLAuth/count-off/AccountIdRecords.txt");
   fstream fstr("/home/hbm/MyShares/tlcomp/201711TLAuth/count-off/AccountIdRecords.txt");
   string line;
   for(WORD32 i = 0; i < ACCOUNT_ID_ROWS; i++)
   {
	   //ifstr.getline(accntId.accChars[i], ACCOUNT_ID_COLS);

	   getline(fstr, line);
	   cout << "Current Line:" << line << endl;
	   accntId.saveAccntCharsWithLine(i, line);
//	   line.copy(accntId.accChars[i], ACCOUNT_ID_COLS);
   }

   accntId.dumpAccntChars();

   ////////////////////////////////////////////////////////////////////
   //扫描每个字符，计算出对于值(bitset值对于的真正的数字(1到９))，有效位数，
   WORD32 startCol = 0;
   for (WORD32 digitNum = 0; digitNum < ACCOUNT_ID_DIGIT_NUM; digitNum++)
   {
	   string digitString = "";
	   AccountDigit accDigit;
	   startCol = digitNum * ACCOUNT_DIGIT_COLS;

	   for (WORD32 rowIdx = 0; rowIdx < ACCOUNT_DIGIT_ROWS; rowIdx++)
	   {
		   for (WORD32 colIdx = startCol; colIdx < (startCol + ACCOUNT_DIGIT_COLS); colIdx++)
		   {
			   char bitChar = accntId.accChars[rowIdx][colIdx];
			   if (bitChar == ' ')
			   {
				   digitString += "0";
			   }
			   else if (bitChar == '_' || bitChar == '|')
			   {
				   digitString += "1";
				   //需统计有效位为１的Pos集合
			   }
			   else
			   {
				   cout << "Fatal Error:Unrecognized illegal Character:" << bitChar << endl;
				   return 0xFF;
			   }
		   }
	   }

	   bitset<ACCOUNT_DIGIT_BIT_SIZE> digitBitSet(digitString);
	   accDigit.bitMapValue = (WORD32)digitBitSet.to_ulong();

	   WORD32 digitValue = convertDigitCharToNum(accDigit.bitMapValue);
	   accDigit.status = (WORD32)STATUS_NORMAL;
	   accDigit.realNum = digitValue;
	   accDigit.bitsetChar = digitBitSet;

	   if (digitValue == DIGIT_INVALID_VALUE)
	   {
		   accDigit.status = (WORD32)STATUS_ILL;
		   cout << "Invalid  digitValue:" << digitValue << endl;
	   }

	   accntId.saveAccntDigit(accDigit);
   }

   ////////////////////////////////////////////////////////////////////
   //对每个字符值进行校验，若校验通过，则打印，否则打印出错ERR
   cout << "Check Account ID Passed: " << accntId.checkAccntSum() << endl;
   ////////////////////////////////////////////////////////////////////


   ////////////////////////////////////////////////////////////////////


   ////////////////////////////////////////////////////////////////////


   ////////////////////////////////////////////////////////////////////





   return RUN_ALL_TESTS();;
}

