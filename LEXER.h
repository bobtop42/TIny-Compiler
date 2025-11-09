#ifndef LEXER_H
#define LEXER_H
#include "token.h"


class LEXER
{
public:
TOKEN<int> tokenChain;
uint8_t flags = 0x00;
tin::string code = "";
tin::string temp = "";
tin::string temp2 = "";
int *tokenCounter = &tokenChain.tokenCounter;
int i; int tL=0;
/*0x01 = intstr, 0x02 = backslash, 0x04 = in if/while/for etc, 0x04 = compilation error, 0x10 = extra flag*/

int max = 0;
int openp = 0;

bool initVar();
void initString(char ch);


bool initLogicComp(uint8_t dubEq);
bool initOperator(uint8_t opEqual);
bool boolCheck();
bool initBrackets();


uint8_t initConst(uint8_t constflags);
bool initConst();

uint8_t initStatic(uint8_t staticflags);
bool initStatic();

bool initSwitch1();
bool initSwitch();
bool initCase();

bool initWhile();
bool initFor();
bool initIf();
bool initElse();
bool initGoto();
bool initLable();
bool initInline();

bool initContinue();

bool readVarName(char ch, bool asign);
bool readVarName(uint8_t varflags, int maxnum);
bool initNamespace();

inline void checkClr(int add);

bool initType();
bool initNum(uint8_t numflags);
bool initNumType(uint8_t numflags);

bool initIncAndDec();
bool initLogicOperator();

void initCode();
void clear(int num, bool clrtemp);
void lexer();


};

#endif
