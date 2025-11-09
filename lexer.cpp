#include "LEXER.h"

void LEXER::initCode()
{
  int t = tokenChain.code.len();
  t--;
  for(int i = 0; i <= t; ++i)
    {
      code+=tokenChain.code[i];
    }
}

bool LEXER::initVar()
{
  checkClr(0);
  if(code.substr(i,1)=="&")
  {
    tokenChain.push_front(TOKENTYPE::adrof);
    clear(1, true); return true;
  }
  else if(code.substr(i,1)=="*")
  {
    tokenChain.push_front(TOKENTYPE::deref);
    clear(1, true); return true;
  }
  return false;
}

inline
void LEXER::checkClr(int add)
{
  if (code.substr(i+add,1)==" "){i+=1+add;}
}

void LEXER::clear(int add, const bool check)
{
  if(check == true){temp="";}
  i+=add; tokenCounter++;
}

inline
bool LEXER::initOperator(const uint8_t opEqual)
{
  checkClr(0);
  temp=code.substr(i,1);
  TOKENTYPE* type = nullptr;/* check this too*/
  if(temp=="+"){*type = TOKENTYPE::addit;}
  else if(temp=="-"){*type = TOKENTYPE::subtr;}
  else if(temp=="*"){*type = TOKENTYPE::multp;}
  else if(temp=="/"){*type = TOKENTYPE::divid;}
  else if(temp=="%"){*type = TOKENTYPE::modul;}
  else if(temp=="&"){*type = TOKENTYPE::btand;}
  else if(temp=="|"){*type = TOKENTYPE::_btor;}
  else if(temp=="^"){*type = TOKENTYPE::btxor;}
  else if(temp=="~"){*type = TOKENTYPE::btnot;}
  else if(temp=="<")
  {
    if (code.substr(i+1,1)=="<"){*type = TOKENTYPE::btshl;}
  }
  else if(temp==">")
  {
    if (code.substr(i+1,1)==">"){*type = TOKENTYPE::btshr;}
  }
  if(0x01==(0x01&opEqual) && type != nullptr) 
  {
    tokenChain.push_front(*type);
    clear(1, true);
    return true;
  }
  else if(0x02==(0x02&opEqual) && type != nullptr)
  {
    if(code.substr(i+1,1)=="=")
    {
      type+=1;
      tokenChain.push_front(*type);
      if(TOKENTYPE::btshl==*type||TOKENTYPE::btshr==*type){i++;}
      clear(2, true);
      return true;
    }
  }
  else if(nullptr!=type)
  {
    if(code.substr(i+1,1)=="=")
    {
      type+=1;
      tokenChain.push_front(*type);
      if(TOKENTYPE::btshl==*type||TOKENTYPE::btshr==*type){i++;}
      clear(2, true);
      return true;
    }
    else
    {
      tokenChain.push_front(*type);
      if(TOKENTYPE::btshl==*type||TOKENTYPE::btshr==*type){i++;}
      clear(1, true);
      return true;
    }
  }
  return false;
}

bool LEXER::initLogicOperator()
{
  tin::string t = code.substr(i,2);
  if(t=="&&")
  {
    tokenChain.push_front(TOKENTYPE::logan);
    clear(2, true); return true;
  }
  else if(t=="||")
  {
    tokenChain.push_front(TOKENTYPE::logor);
    clear(2, true); return true;
  }
  else if(t=="==")
  {
    tokenChain.push_front(TOKENTYPE::equal);
    clear(2, true); return true;
  }
  return false;
}

bool LEXER::initLogicComp(uint8_t dubEq)
{
  checkClr(1);
  temp=code.substr(i,1);
  if(0x01==(dubEq&0x01)&&code.substr(i+1,1)=="=")
  {
    if(temp=="="){tokenChain.push_front(TOKENTYPE::asign);}
    else if(temp=="!"){tokenChain.push_front(TOKENTYPE::noteq);}
    else if(temp==">"){tokenChain.push_front(TOKENTYPE::moree);}
    else if(temp=="<"){tokenChain.push_front(TOKENTYPE::lesse);}
    else{return false;}
    clear(2, true);
    return true;
  }
  else if(code.substr(i+1,1)!="=")
  {
    if(temp=="="){tokenChain.push_front(TOKENTYPE::equal);}
    else if(temp=="!"){tokenChain.push_front(TOKENTYPE::logno);}
    else if(temp==">"){tokenChain.push_front(TOKENTYPE::moret);}
    else if(temp=="<"){tokenChain.push_front(TOKENTYPE::lesst);}
    else{return false;}
    clear(1, true);
    return true;
  }
  return false;
}

bool LEXER::boolCheck()
{
  checkClr(0);
  if(code.substr(i,4)=="true")
  {
    tokenChain.push_front(TOKENTYPE::boolt);
    clear(4, true);
    return true;
  }
  else if(code.substr(i,5)=="false")
  {
    tokenChain.push_front(TOKENTYPE::boolf);
    clear(5, true);
    return true;
  }
  return false;
}

void LEXER::initString(char ch)
{
  if(0xFF==static_cast<unsigned char>(ch)) {ch = code[i];}
  clear(1, true);
  while(code.substr(i,1)!="\""&&code.substr(i-1,1)!="\\")
    {
      temp+=code[i]; i++;
    }
  tokenChain.push_front(temp,TOKENTYPE::strng);
  clear(1, true);
}

bool LEXER::initType()
{
  TOKENTYPE tokentemp = TOKENTYPE::Nlptr; int clrAm = 0; bool derefCheck = false;
  if(code.substr(i+1,3)=="int")
    {
      tokentemp = TOKENTYPE::__int; clrAm = 4;
    }
    else if(code.substr(i+1,5)=="float")
    {
      tokentemp = TOKENTYPE::Float; clrAm = 6;
    }
    else if(code.substr(i+1,4)=="long")
    {
      tokentemp = TOKENTYPE::_long; clrAm = 5;
    }
    else if(code.substr(i+1,6)=="double")
    {
      tokentemp = TOKENTYPE::doubl; clrAm = 7;
    }
    else if(code.substr(i+1,4)=="char")
    {
      clrAm = 5;
      if(code.substr(i+clrAm,1)==" "||code.substr(i+clrAm,1)=="*")
      {
        tokentemp = TOKENTYPE::_char;
      }
      else if(code.substr(i+clrAm,3)=="8_t")
      {
        tokentemp = TOKENTYPE::char8; clrAm += 3;
      }
      else if(code.substr(i+clrAm,4)=="16_t")
      {
        tokentemp = TOKENTYPE::chr16; clrAm += 4;
      }
      else if(code.substr(i+clrAm,4)=="32_t")
      {
        tokentemp = TOKENTYPE::chr32; clrAm += 4;
      }
      else
      {
        return false;
      }
    }
    else if(code.substr(i+clrAm,4)=="bool")
    {
      tokentemp = TOKENTYPE::_bool; clrAm = 5;
    }

    if(tokentemp!=TOKENTYPE::Nlptr)
    {
      if(code.substr(i+clrAm, 1)==" ")
      {
        tokenChain.push_front(tokentemp);
        clear(clrAm, true);
        derefCheck = initVar();
        if(derefCheck)
        {
          clear(1, true);
        }
        checkClr(0);
        return true;
      }
      else if(code.substr(i+clrAm, 1)=="*")
      {
        tokenChain.push_front(tokentemp);
        clear(clrAm, true);
        tokenChain.push_front(TOKENTYPE::deref);
        clear(1, true);
        checkClr(0);
        return true;
      }
      else if(code.substr(i+clrAm, 1)=="&")
      {
        tokenChain.push_front(tokentemp);
        clear(clrAm, true);
        if(code.substr(i+1, 1)=="&")
        {
          tokenChain.push_front(TOKENTYPE::drval);
          i++;
        }
        else
        {
          tokenChain.push_front(TOKENTYPE::_lval);
        }
        clear(1, true);
        checkClr(0);
        return true;
      }
    }
    return false;
}

bool LEXER::initNum(uint8_t numflags)
{
  /*0x01=ints, 0x02=floats, 0x04=unsigned, 0x08=doubles, 0x10=char, 0x80=hexflags(SET IN FUNC ONLY)*/

  bool check1;
  
  if (0x01==(numflags&0x01)||0x10==(numflags&0x10))
  {
    if(code.substr(i,2)=="0x"){numflags|=0x80; i+=2;}
    /*see if the "-" will asign in the readVarName*/
    if(code.substr(i,1)=="-"&&(0x80==(numflags&0x80)||0x04==(numflags&0x04)))
    {
      flags|=0x40; return false;
    }
    /*check if it stops on periods for floats and doubles*/
    
    check1 = readVarName(0x01, 6); /*check if this is the right amount of digits for the num types in this part*/

    if(4<tL&&0x80==(numflags&0x80)){flags|=0x40; return false;}
    return true;
  }
  else if(0x02==(numflags&0x02)||0x08==(numflags&0x08))
  {
    if(code.substr(i,2)=="0x")
    {
      std::cout<<"ERROR: hexadecimal is not supported for floats and doubles types"<<std::endl;
      flags|=0x40;
      return false;
    }
    if(code.substr(i,1)=="-"&&0x04==(numflags&0x04))
    {
      flags|=0x40; return false;
    }
    check1 = readVarName(0x01, 15);
    /*remember to have a "f"/"F" check for floats*/
    return true;
  }
  return false;
}

bool LEXER::initNumType(uint8_t numflags)
{
  uint8_t num; bool charAndIntCheck; bool floatAndDoubleCheck;
  if(0x01==(numflags&0x01)||0x10==(numflags&0x10))
  {
    num = numflags&0x15;
    charAndIntCheck = initNum(num);
  }
  else if(0x0A==(numflags&0x0A))
  {
    num = numflags&0x0E;
    floatAndDoubleCheck = initNum(num);
  }
  if(0x40==(flags&0x40)){return false;}
  if(charAndIntCheck||floatAndDoubleCheck){return true;}
  return false;
}

bool LEXER::initIncAndDec()
{
  tin::string t;//should be tin;;string
  t = code.substr(i,2); //code var is a tring. check the ==
  if(t=="++")
  {
    tokenChain.push_front(TOKENTYPE::incra);
    clear(2, true);
    return true;
  }
  else if(t=="--")
  {
    tokenChain.push_front(TOKENTYPE::decre);
    clear(2, true); return true;
  }
  return false;  
}

bool LEXER::initBrackets()
{
  char ch = temp[0];
  switch(ch)
    {
      case '(':
        {
          tokenChain.push_front(TOKENTYPE::openp);
          clear(1, true); max++; openp++; return true;
        }
      case ')':
        {
          tokenChain.push_front(TOKENTYPE::closp);
          clear(1, true); max++; openp--; return true;
        }
      case '[':
        {
          tokenChain.push_front(TOKENTYPE::openb);
          clear(1, true); max++;
          bool c1 = initNumType(0x1B);
          if(!c1){c1 = initNumType(0x1F);}
          if(!c1){c1 = readVarName(0x01,100);}
          return true;
        }
      case ']':
        {
          tokenChain.push_front(TOKENTYPE::closb);
          clear(1, true); max++; return true;
        }
      case '{':
        {
          tokenChain.push_front(TOKENTYPE::openb);
          clear(1, true); max++; return true;
        }
      case '}':
        {
          tokenChain.push_front(TOKENTYPE::closb);
          clear(1, true); max++; return true;
        }
      default:
        {
          return false;
        }
    }
}

bool LEXER::readVarName(char ch, bool asign)
{
  if(0xFF==ch){ch = code[i];}
  if(ch>=0&&ch<31||ch>=34&&ch<=36||ch==39||ch==64||ch==92||ch==96||ch==127)
  {
    std::cout<<"ERROR: can not use \""<<ch<<"\" in a variable name"<<std::endl;
    flags|=0x40;
    return false;
  }
  else if(ch==32||ch==33||ch>=37&&ch<=47&&ch!=39||ch>=58&&ch<=63||ch>=91&&ch<=94||ch>=123&&ch<=126)
  {
    if(asign)
    {
      tokenChain.push_front(temp,TOKENTYPE::varbl);
      clear(1, true);
      checkClr(0);
      bool check1 = initOperator(0x04);
      return true;
    }
  }
  else
  {
    i++; tL++;
    return true;/**TEMP IS NOT CLEARED!!!!*/
  }
  flags|=0x40; return false;
}

bool LEXER::readVarName(uint8_t varflags, int maxnum)
{
  flags|=0x01; temp+=code.substr(i,1); max++; tL = 1;
  while(0x00!=(flags&0x40)&&max<maxnum&&0x01==(flags&0x01))
    {
      char ch; bool check2;
      if(0x80 == (varflags&0x80)){ch = 0xFF;}
      else{ch = code[i];}
      if(0x01==(varflags&0x01)){check2 = readVarName(ch, true);}
      else{check2 = readVarName(ch, false);}
      tL++;
      if(!check2){temp+=ch; max++;}
      else{flags^=0x01; tL--; return true;}
    }
  return false;/*went over max char count*/
}

uint8_t LEXER::initStatic(uint8_t staticflags)
{
  if(code.substr(i,1)==" ")
  {
    tokenChain.push_front(TOKENTYPE::statc);
    clear(1, true);
    return (staticflags|0x01);
  }
  temp2=code.substr(i,5);
  if(temp2=="_cast")
  {
    i+=5;
    if(code.substr(i,1)!="<"){return staticflags;}/*add a warning check?*/
    i++;
    bool typeCheck = initType();
    if(!typeCheck)      
    {
      std::cout<<"ERROR: incorrect init of static_cast"<<std::endl;
      return (staticflags|0x80);
    }
    if(code.substr(i,1)!=">")
    {
      std::cout<<"ERROR: must have a closing \">\" in a static_cast"<<std::endl;
      return (staticflags|0x80);
    }
    i++;
    typeCheck = initBrackets();
    if(!typeCheck)
    {
      std::cout<<"ERROR: incorrect init of static_cast"<<std::endl;
      return (staticflags|0x80);
    }
    return (staticflags|0x02);
  }
  else if(temp=="_asse"&&code.substr(i+5,2)=="rt")
  {
    std::cout<<"ERROR: static_assert is not currently supported"<<std::endl;
    return (staticflags|0x80);
  }
  return staticflags;
}

bool LEXER::initStatic()
{
  uint8_t staticCheck2 = initStatic(0x00);
  if(0x80==(staticCheck2&0x80)){flags|=0x40; return false;}
  else if(0x01==(staticCheck2&0x01||0x02==(staticCheck2&0x02))){return true;}
  return false;
}

uint8_t LEXER::initConst(uint8_t constflags)
{
  if(code.substr(i,1)==" ")
  {
    tokenChain.push_front(TOKENTYPE::Const);
    clear(1, true); return (constflags|0x01);
  }
  temp2 = code.substr(i+1, 5);
  if(temp2=="eval ")
  {
    tokenChain.push_front(TOKENTYPE::ceval);
    clear(6, true); return (constflags|0x02);
  }
  if(temp2=="init ")
  {
    tokenChain.push_front(TOKENTYPE::cinit);
    clear(6, true); return (constflags|0x04);
  }
  if(temp2=="_cast"&&code.substr(i+6,1)==" ")
  {
    tokenChain.push_front(TOKENTYPE::ccast);
    clear(7, true); return (constflags|0x08);
  }
  return constflags;
}

bool LEXER::initConst()
{
  uint8_t constCheck2 = initConst(0x00);
  if(0x00==(constCheck2&0x00)){flags|=0x40; return false;}
  return true;
}

bool LEXER::initContinue()
{
  if(code.substr(i+1,1)==" ")
  {
    tokenChain.push_front(TOKENTYPE::contn);
    clear(2, true); return true;
  }
  return false;
}


/*im very sorry about the code below. pls forgive me*/
bool LEXER::initIf()
{
  openp++; max = 0;
  if(code.substr(i+1,1)!="("&&code.substr(i+1,2)!=" (")
  {
    return false;
  }
  tokenChain.push_front(TOKENTYPE::___if);
  checkClr(1); clear(1, true);
  flags|=0x04; max = 0;
  while(0x04==(flags&0x40)||86>max)
    {
      temp+=code.substr(i,1); max++;
      if(temp=="\""){initString(0xFF);}
      bool incAndDecCheck =initIncAndDec();
      bool init1 = initOperator(0x01);
      if(init1){max++;}
      else
      {
        init1 = initBrackets();
        if(init1){max++;}
        else
        {
          init1 = initLogicComp(0x00);
          if(init1){max++;}
          else
          {
            init1 = initLogicComp(0x01);
            if(init1){max++;}
            else
            {
              init1 = initLogicOperator();
              if(init1){max++;}
              else
              {
                init1 = initNumType(0x1B);
                if(init1){max++;}
                else
                {
                  init1 = initNumType(0x1F);
                  if(init1){max++;}
                  else
                  {
                    init1 = readVarName(0x01, 86);
                  }
                }  
              }
            }
          }
        }
      }
      max+=tL; clear(tL, true); tL = 0;
      checkClr(0);
    }
  if(86<=max){flags|=0x40; return false;}
  if(code.substr(i,1)!=")"){flags|=0x40; return false;}
  checkClr(0); clear(1,true);
  openp--; flags^=0x04; max = 0;
  return true;
}

bool LEXER::initElse()
{
  if(code.substr(i,1)==" ")
  {
    tokenChain.push_front(TOKENTYPE::_else);
    clear(1, true);
    bool b1c = initBrackets();
    if(!b1c)
    {
      std::cout<<"ERROR: else statements must have a open and close curly brackets"<<std::endl;
      flags|=0x40; return false;
    }
    return true;
  }
  return false;
}

bool LEXER::initSwitch1()
{
  if(code.substr(i+1,1)==" ")
  {
    tokenChain.push_front(TOKENTYPE::swtch);
    clear(1, true); return true;
  }
  else if(code.substr(i+1,1)=="(")
    {
      tokenChain.push_front(TOKENTYPE::swtch);
      clear(1, true); checkClr(0);
      return true;
    }
  return false;
}

bool LEXER::initSwitch()
{
  bool initSwitchCheck = initSwitch1();

  try
    {
      if(!initSwitchCheck){throw 0x01;}

      initSwitchCheck = initBrackets();
      if(!initSwitchCheck){throw 0x01;}
      checkClr(0);
      initSwitchCheck = initNumType(0x17);
      if(!initSwitchCheck)
      {
        initSwitchCheck = readVarName(0x01, 12);
        if(initSwitchCheck)
        {
          initSwitchCheck = initBrackets();
          if(initSwitchCheck)
          {
            initSwitchCheck = initBrackets();
            if(!initSwitchCheck){throw 0x02;}
          }
          else
          {
                  if(code.substr(i,1)!="\""&&code.substr(i,1)!="\'"){throw 0x02;}
            i++;
            bool varCheck = readVarName(0x02, 12);
            if(!varCheck){throw 0x02;}
            checkClr(0);

            tokenChain.push_front(temp,TOKENTYPE::varbl);
            clear(0, true);

            if(code.substr(i,1)!=")"){throw 0x02;}
            varCheck = initBrackets();
          }
        }
        else
        {
          throw 0x02;
        }
      }
      else
      {
        checkClr(0);
        if(code.substr(i,1)!=")"){throw 0x02;}
        i++;
      }
      return true;
    }
    catch(int e)
    {
      if(0x01==e)
      {
        std::cout<<"ERROR: incorrect switch statement setup"<<std::endl;
      }
      flags|=0x40;
      return false;
    }
    catch(...)
    {
      std::cout<<"ERROR: unknown error"<<std::endl;
      flags|=0x40;
      return false;
    }
}

bool LEXER::initCase()
{
  if(code.substr(i,1)==" "){i++;}
  tokenChain.push_front(TOKENTYPE::_case);
  clear(1, true);
  bool numCheck = initNumType(0x17);
  if(!numCheck)
  {
    numCheck = readVarName(0x01, 12);
    if(!numCheck)
    {
      numCheck = initBrackets();
      if(numCheck)
      {
        numCheck = initBrackets();
        if(!numCheck)
        {
          flags|=0x40;
          std::cout<<"ERROR: must close brakces/parentheses etc."<<std::endl;
          return false;
        }
      }
      else
      {
        if(code.substr(i,1)!="\'"&&code.substr(i,1)!="\""){flags|=0x40;return false;}
        bool varCheck = readVarName(0x01, 12);
        if(!varCheck){flags|=0x40; return false;}
        checkClr(0);

        tokenChain.push_front(temp,TOKENTYPE::varbl);
        clear(0, true);
        if(code.substr(i,1)!=":"){flags|=0x40; return false;}
        varCheck = initBrackets();
        if(!varCheck){flags|=0x40; return false;}
        
      }
    }
    else{flags|=0x40; return false;}
  }
  else
  {
    if(code.substr(i,1)!=":"){flags|=0x40; return false;}
    numCheck = initBrackets();
    if(!numCheck){flags|=0x40; return false;}
  }
  return true;
}

bool LEXER::initWhile()
{
  checkClr(1);/*check the i+1*/
  if(code.substr(i,1)!="("){flags|=0x40; return false;}
  tokenChain.push_front(TOKENTYPE::While);
  clear(1, true);

  openp = 1; flags|=0x10; max = 0;

  while(0x00!=(flags&0x40)||max<32)
    {
      bool numCheck = initNumType(0x1B);
      if(!numCheck)
      {
        numCheck = initNumType(0x1F);
        if(!numCheck)
        {
          numCheck = readVarName(0x01, 24);/*32 chars max*/
          if(!numCheck)
          {
            std::cout<<"ERROR must have a comparison"<<std::endl;
            flags|=0x40;/*add support for !varNameEx*/
          }
          else
          {
            max+=tL;
            clear(tL, true);
            numCheck = initBrackets();
          }
        }
      }
      checkClr(0);
      bool logicCompCheck = initLogicComp(0x01);
      if(!logicCompCheck)
      {
        std::cout<<"ERROR: must have a comparison"<<std::endl;
        flags|=0x40; return false;
      }
      checkClr(0);

      numCheck = readVarName(0x01, 32);
      if(!numCheck){numCheck = initNumType(0x1B);}
      if(!numCheck)
      {
        std::cout<<"ERROR: must have a val/var to compare"<<std::endl;
        flags|=0x40; return false;
      }
      max+=tL; clear(tL, true);
      checkClr(0);
      if(code.substr(i,1)==")"){flags^=0x10; break;}
    }
  if(max>32){flags|=0x40; return false;}
  max = 0; return true;
}

bool LEXER::initFor()
{
  openp++; flags|=0x10; max = 0;

  try
    {
      if(code.substr(i,1)!="("){throw "must have an open parentheses";}
      tokenChain.push_front(TOKENTYPE::__for);
      clear(1, true);
      checkClr(0);
      flags|=0x04;

      bool typeCheck = initType(); max+=4;
      if(!typeCheck){throw "must have a type";}

      bool varNameCheck = readVarName(0x01, true);
      if(!varNameCheck){throw "must have a variable name";}
      max+=tL; tL=0;

      if(32<=max){throw "for loop max char count exeeded";}

      tokenChain.push_front(TOKENTYPE::semic);
      /*fix below*/
      if(code.substr(i,2)==" ;" || code.substr(i,2)==";"){clear(2, true);}
      else if(code.substr(i,1)==";"|| code.substr(i,1)==" "){clear(1, true);}
      else{throw "must have a semicolon at the end of a line";}
      tL++;

      bool numCheck = initNumType(0x1B);/*check initNumType etc for right func and func vars*/
      if(!numCheck)
      {
        numCheck=initNumType(0x1F);
        if(!numCheck){numCheck=readVarName(0x01, 32);}
        if(!numCheck){throw "must have a comparison";}
      }
      max+=tL; clear(tL, true);
      
      tL=0;/*check this*/
      checkClr(0);

      bool logicCompCheck = initLogicComp(0x00);
      if(!logicCompCheck){logicCompCheck = initLogicComp(0x01);}
      if(!logicCompCheck){throw "must have a comparison";}

      checkClr(0);

      numCheck = readVarName(0x01, 32);
      if(!numCheck)
      {
        numCheck = initNumType(0x1B);
        if(!numCheck){ numCheck=initNumType(0x1F);}
        if(!numCheck){throw "must have a val/var to compare";}
      }
      max+=tL; clear(tL, true); checkClr(0);

      if(32<=max){throw "for loop max char count exeeded";}

      tokenChain.push_front(TOKENTYPE::semic);
      
      if(code.substr(i,2)==" ;" || code.substr(i,2)=="; "){clear(2, true);}
      else if(code.substr(i,1)==";"){clear(1, true);}
      else{throw "must have a semicolon at the end of a line";}
      tL++;

      checkClr(0);

      numCheck = readVarName(0x01, 32);
      if(!numCheck){throw "must have a variable name";}
      max+=tL; clear(tL, true);

      numCheck = initIncAndDec();
      if(!numCheck)
      {
        numCheck = initOperator(0x02);
        if(!numCheck){throw "must change the val in a for loop(no current suppor for pre ++/-- and for loops w/o func body)";}
        numCheck = initNumType(0x1B);
        if(!numCheck)
        {
          numCheck = initNumType(0x1F);
          if(!numCheck){throw "must incrament or decrament a val in a for loop";}
          max+=tL; clear(tL, true);
        }
        checkClr(0);
        if(code.substr(i,1)!=")"){throw "must have a close parentheses";}
        openp--; flags^=0x10;
        return true;
      }
      
    }
    catch(const char* msg)
    {
      std::cout<<"ERROR: "<<msg<<std::endl;
      flags|=0x40;
      return false;
    }
    catch(...)
    {
      std::cout<<"ERROR: unknown error in for loop..."<<std::endl;
      flags|=0xD0;
      return false;
    }
  return false;
}

bool LEXER::initGoto()
{
  if(code.substr(i+1,1)!=" ")
  {
    return false;
  }
  tokenChain.push_front(TOKENTYPE::_goto);
  clear(2, true);
  if(code.substr(i,1)!=" ")
  {
    std::cout<<"ERROR: must have a space after a goto statement"<<std::endl;
    flags|=0x40; return false;
  }
  i++;
  bool initGotoLableName = readVarName(0x81, 12);
  if(!initGotoLableName)
  {
    std::cout<<"ERROR: must have a lable name after init of a goto statement"<<std::endl;
    flags|=0x40; return false;
  }
  checkClr(0);
  if(code.substr(i,1)!=";")
  {
    std::cout<<"ERROR: must have a semicolon at the end of a line(got staement)"<<std::endl;
    flags|=0x40; return false;
  }
  tokenChain.push_front(TOKENTYPE::semic);
  clear(1, true);
  return true;
  
}

bool LEXER::initNamespace()
{
  if(code.substr(i+1,1)!=" ")
  {
    return false;
  }
  tokenChain.push_front(TOKENTYPE::nmspc);
  clear(2, true);
  bool namespaceCheck = readVarName(0x81,12);
  if(!namespaceCheck)
  {
    std::cout<<"ERROR: must have a namespace name"<<std::endl;
    flags|=0x40; return false;
  }
  checkClr(0);
  return true;
}

bool LEXER::initInline()
{
  if(code.substr(i+1,1)!=" ")
  {
    return false;
  }
  tokenChain.push_front(TOKENTYPE::inlin);
  clear(2,true);
  return true;
}

void LEXER::lexer()
{
  /*0x01 = intstr, 0x02 = backslash, 0x04 = in if/while/for etc, 0x04 = compilation error, 0x10 = extra flag*/

  /*flags, temp, i(index), and ti (temp index) are in class init*/
  int codelen = code.len(); int templen = 0;
  while(i<codelen||0x40!=(flags&0x40))
    {
      temp+=code.substr(i,1);
      if(temp=="\""){initString(0xFF); continue;}
      templen = temp.len();
      if(1<templen)
      {
        if(templen==2)
        {
          if(temp=="if"){bool forCheck = initIf(); continue;}
        }
        else if(templen==3)
        {
          if(temp=="for"){bool forCheck = initFor(); continue;}
        }
        else if(templen==4)
        {
          if(temp=="else"){bool elseCheck = initElse(); continue;}
          else if(temp=="case"){bool caseCheck = initCase(); continue;}
          else if(temp=="goto"){bool gotoCheck = initGoto(); continue;}
        }
        else if(templen==5)
        {
          if(temp=="while"){bool whileCheck = initWhile(); continue;}
          else if(temp=="const"){bool constCheck = initConst(); continue;}
          else if(temp=="const"){bool constCheck = initConst(); continue;}
        }
        else if(templen==6)
        {
          if(temp=="switch"){bool switchCheck = initSwitch(); continue;}
          else if(temp=="static"){bool staticCheck = initStatic(); continue;}
          else if(temp=="inline"){bool inlineCheck = initInline(); continue;}
        }
        else if(templen==8)
        {
          if(temp=="continue"){bool continueCheck = initContinue(); continue;}
        }
        else if(templen==9)
        {
          if(temp=="namespace"){bool namespaceCheck = initNamespace(); continue;}
        }
      }
    }
}
