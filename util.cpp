/***********************************************************

util.cpp 

Various utility functions.

************************************************************/

#include "util.h"

/*
    Assists with password input masking.

    taken from:
    http://www.cplusplus.com/articles/E6vU7k9E/
*/

int getch() {
    int ch;
    struct termios t_old, t_new;

    tcgetattr(STDIN_FILENO, &t_old);
    t_new = t_old;
    t_new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
    return ch;
}

/*
    Assists with password input masking.

    taken from:
    http://www.cplusplus.com/articles/E6vU7k9E/
*/
string getpass(const char *prompt, bool show_asterisk)
{
  const char BACKSPACE=127;
  const char RETURN=10;

  string password;
  unsigned char ch=0;

  cout << prompt << endl;

  while((ch=getch())!=RETURN)
    {
       if(ch==BACKSPACE)
         {
            if(password.length()!=0)
              {
                 if(show_asterisk)
                 cout <<"\b \b";
                 password.resize(password.length()-1);
              }
         }
       else
         {
             password+=ch;
             if(show_asterisk)
                 cout <<'*';
         }
    }
  cout << endl;
  return password;
}


/*
	Check existence of a specified file (by opening it).
*/
bool fileExists(const string& name)
{
     ifstream f(name.c_str());
     return f.is_open();
     f.close();
}
