#ifndef TOKEN_H
#define TOKEN_H
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include "tinstring.h"
#include <unistd.h>
#include <variant>

enum class TOKENTYPE:int
{
asign = 0, equal, noteq, moret, moree, lesst, lesse, logor, logan, logno, multp, muleq, divid, diveq, modul, modeq, addit, addeq, subtr, subeq, btand, bande, _btor, boreq, btxor, bxore, btshl, bshle, btshr, bshre, btnot, bnote, incra, decre, 
_sizeo, deref, adrof, memac, ptmem, 
terny, comma, scprs, mema2, ptmm2, 
stcst, dyncs, cncst, reict, 
__new, newar, __del, delar, stprg/*prg start*/, edprg /*prg end*/, nmspc/*namespace*/,



boolt, boolf,

openp/*(*/, closp, openb/*[*/, closb, opnbr/*{*/, clsbr,

swtch, _case, While, __for, ___if, _else, _goto, lable, inlin,

__int, Float, _long, Short, doubl, _char, char8, chr16, chr32, _bool,

varbl, strng, semic/*semicolon*/, statc, contn, 

Const, ceval, cinit, ccast,

Nlptr, _NULL, _lval, flval/*lval->&*/, _rval, drval/*double rval->(&&)*/
};

template<typename T>
class TOKEN
{
public:
TOKENTYPE tokenType;

TOKEN<T>(){}
TOKEN(const TOKEN<T> *other){setNewTokenList();}
~TOKEN(){clearAll();}

struct token
{
  token* prev_;
  tin::string data = "";
  token* next_;
  static TOKENTYPE type; /*check this code*/

  token& operator=(const token& other)
  {
    prev_ = other.prev_;
    data = other.data;
    next_ = other.next_;
    type = other.type;
    return *this;
  }
  void setData(tin::string Data){this->data = Data;}
};

tin::string filename = "";
tin::string code = "";
int tokenCounter = 0;

token* start_ = nullptr;

void push_front(tin::string Data);
void push_front(tin::string Data, TOKENTYPE type);
void push_front(TOKENTYPE type);

void pop_front();

void setNewTokenList();
void clearAll();

void readFile(tin::string fileName);
};


template <typename T> 
void TOKEN<T>::push_front(tin::string Data) 
{
  TOKEN::token *newToken = new TOKEN::token;
  newToken->data = Data;
  newToken->prev_ = start_;
  newToken->next_ = nullptr;
  if (start_ != nullptr)
    start_->next_ = newToken;
  start_ = newToken;
  start_ = *newToken;
};

template <typename T>
void TOKEN<T>::push_front(tin::string Data,TOKENTYPE type) 
{
  TOKEN::token *newToken = new TOKEN::token;
  newToken->data = Data;
  newToken->type = type;
  newToken->prev_ = start_;
  newToken->next_ = nullptr;
  if (start_ != nullptr)
    start_->next_ = newToken;
  start_ = newToken;
  start_ = *newToken;
}

template <typename T> 
void TOKEN<T>::push_front(TOKENTYPE type) 
{
  TOKEN::token *newToken = new TOKEN::token;
  newToken->type = type;
  newToken->prev_ = start_;
  newToken->next_ = nullptr;
  if (start_ != nullptr)
    start_->next_ = newToken;
  start_ = newToken;
}

template <typename T> 
void TOKEN<T>::setNewTokenList()
{
  push_front("@startprg", TOKENTYPE::stprg);
}

template <typename T> void pop_front() {}

template <typename T> void TOKEN<T>::readFile(tin::string fileName) {
  tin::string endFileStr = "";
  uint8_t flags = 0x00;
  // endFileFlag = 0x08, ENDFILETRUE = 0x08, comment = 0x01, checkCOmmentBefore
  // = 0x0x2, esc = 0x02
  char buf[256];
  char temp[7] = {};
  std::ifstream file(fileName);
  int atindex = 0;
  int escindex = 0;
  int tempindex = 0;
  if (file.is_open()) {
    while ((flags & 0x80) != 0x80) {
      int n = read(fileName, buf, sizeof(buf)); // work on this

      if (n > 0) 
      {
        endFileStr = "";
        for (auto t : temp) 
        {
          if ('@' == temp[t]) 
          {
            atindex = t;
            flags |= 0x08;
          }
          if ((flags & 0x08) == 0x08) 
          {
            endFileStr += temp[t];
          }
        }
        if (endFileStr=="@endprg") 
        {
          flags |= 0x80;
        }
        if (0x08 == (flags & 0x08)) 
        {
          for (int t; t < 6; ++t) 
          {
            endFileStr += buf[t];
            if (endFileStr=="@endprg") 
            {
              flags |= 0x80;
              break;
            }
          }
          flags ^= 0x08;
        }
        for (int i : buf) 
        {
          if (buf[i] == '\\') 
          {
            flags |= 0x04;
            escindex = i;
          }
          if ('/' == buf[i]) 
          {
            if (255 != i && buf[i + 1] == '*' && 0x00 == (flags & 0x04)) 
            {
              flags |= 0x01;
              for (;;) 
              {
                if (i < 254) 
                {
                  if ('*' == buf[i] && '/' == buf[i + 1]) 
                  {
                    i += 2;
                    flags ^= 0x01;
                    continue;
                  }
                } 
                i++;
              }
              if (0x04 == (flags & 0x04)) 
              {
                flags ^= 0x04;
              }
            } 
            else if (0x00 == (flags & 0x04) && 255 == i) 
            {
              flags |= 0x02;
            } else 
            {
              code += buf[i];
            }
            if (0x01 == (flags & 0x01)) 
            {
              flags ^= 0x01;
            }
          } 
          else if ('@' == buf[i] && 0x00 == (flags & 0x04)) 
          {
            if (i < 249) 
            {
              endFileStr = "";
              for (int j = 0; j < 7; ++j) 
              {
                endFileStr += buf[i + j];
              }
              if (endFileStr=="@endprg") 
              {
                flags |= 0x80; break;
              }
            } 
            else
            {
              int j = 0;
              while (i < 256) 
              {
                *(temp + j++) = *(buf + j + i++);
              }
            }
          } 
          else if (0x80 == (flags & 0x80)) 
          {
            close(filename);
            break;
          } 
          else if (i > 248) 
          {
            code += temp[i - 249] = buf[i];
          } 
          else if (0x00 == (flags & 0x01) && ' ' == buf[i]) 
          {
            while (' ' == buf[i]) 
            {
              i++;
            }
          } 
          else 
          {
            code += buf[i];
          }
        }
      } 
      else 
      {
        close(filename);
        std::cout << "Error: no file contents found...\nclosing compiler"
                  << std::endl;
        exit(0);
      }
    }
  }
}

#endif
