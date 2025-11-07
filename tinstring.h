#ifndef TIN_H /*TI Nspire*/
#define TIN_H

namespace tin
{
class string 
{
char* data;

/*compute length of C-string (null-terminated)*/
static unsigned length(const char* s) 
{
  unsigned n = 0;
  while (s && s[n]) {++n;};
  return n;
}

// copy source into destination (assumes enough space)
static void copy(char* dest, const char* src) 
{
    while (*src) 
      {
        *dest++ = *src++;
      }
    *dest = '\0';
}

public:
/*ctor from const char* */
string(const char* other = "") 
{
  if (!other) 
  {
    /*null pointer guard*/
    data = new char[1];
    data[0] = '\0';
    return;
  }

  unsigned len = length(other);
  data = new char[len + 1];
  copy(data, other);
}

/*copy ctor*/
string(const string& other) : string(other.data) {}

/*detor*/
~string() {if (data){delete[] data;}}

const char* str() const {return data;}

  /*check the const part of the [] two below*/
char  operator[](int index) { return *(data + index); }
char &operator[](int index) const { return *(data + index);}

string& operator=(string&&other)
{
  if(*this != other)
  {
      delete[] data;
      data = other.data;
      other.data = nullptr;
  }
  return *this;
}

bool operator==(const string &other) 
{
  int o = other.len();
  int t = this->len();
  if (o != t) {return false;}

  for (int i = 0; i < o; i++) 
  {
    if (data[i] != other[i]) {return false;}
  }
  return true;
}

bool operator!=(const string &other) { return (!(*this == other));}

string operator+=(const string &other) 
{
  int o = other.len();
  int t = this->len();

  char *oldData = data;
  data = new char[o + t + 1];

  for (int i = 0; i < t; i++) 
    {
      data[i] = oldData[i];
    }
  for (int i = 0; i < o; i++) 
    {
      data[i + t] = other[i];
    }

  data[o + t] = '\0';

  if (oldData) 
  {
    delete[] oldData;
  }
  return *this;
}

string operator+(const string &other) 
{
  *this += other;
  return *this;
}

string substr(int start, int lenofsubstr) 
{
  string substr;
  for (int i = 0; i < lenofsubstr; i++) 
    {
      substr.data[i] = data[i + start];
    }

  substr.data[lenofsubstr] = '\0';
  return substr;
}

int len() const
{
  int i = 0;
  while (*(data + i) != '\0')
    {
      i++;
    }
  return i;
}
};
};

#endif
