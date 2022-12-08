#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>

class Lexer{
  public:

//List of token codes
#define BEGIN_CODE 0
#define END_CODE 1
#define LETTER 2
#define DIGIT 3
#define ASSIGN_OP 10
#define ADD_OP 11
#define SUB_OP 12
#define MULT_OP 13
#define DIV_OP 14
#define MODULUS 15
#define EQUAL_TO 16
#define GREATER_THAN 17
#define LESS_THAN 18
#define UNDERSCORE 19
#define GREATER_THAN_EQUAL 20
#define LESS_THAN_EQUAL 21
#define NOT_EQUAL_TO 22
#define LEFT_PAREN 23
#define RIGHT_PAREN 24
#define INT_LIT 25
#define IDENT 26
#define WHILE 27
#define IF 28
#define ELSE 29
#define VARIABLE 30
#define END_OF_LINE 31
#define CODE_BLOCK_START 32
#define CODE_BLOCK_END 33
#define SPACE 34
#define DOLLA 35
#define ERROR 99

//Define interger literal
//1 byte
#define ONE 35
//2 byte
#define TWO 36
//4 byte
#define FOUR 37
//8 byte
#define EIGHT 38

  //Declare Variables
  int charClass;
  int lexLen;
  char lexeme[100];
  char nextChar;
  int tok[1000];
  int tok_index;
  int nextToken;
  int synerr_counter;
  FILE *in_fp;

//Function lookup to look up token and keyword to return result
  int lookup(char ch) {
    if (isupper(ch)) {
      nextToken = ERROR;
    }
    switch (ch) {
    case '+':
      addChar();
      nextToken = ADD_OP;
      break;
    case '-':
      addChar();
      nextToken = SUB_OP;
      break;
    case '*':
      addChar();
      nextToken = MULT_OP;
      break;
    case '/':
      addChar();
      nextToken = DIV_OP;
      break;
    case '%':
      addChar();
      nextToken = MODULUS;
      break;
    case '=':
      addChar();
      nextToken = ASSIGN_OP;
      break;
    case '<':
      addChar();
      nextToken = LESS_THAN;
      break;
    case '>':
      addChar();
      nextToken = GREATER_THAN;
      break;
    // # for not equal !=
    case '#':
      addChar();
      nextToken = NOT_EQUAL_TO;
      break;
    // ! for less than or equal <=
    case '!':
      addChar();
      nextToken = LESS_THAN_EQUAL;
      break;
    // ^ for greater than or equal >=
    case '^':
      addChar();
      nextToken = GREATER_THAN_EQUAL;
      break;
     // @ for equal to ==
    case '@':
      addChar();
      nextToken = EQUAL_TO;
      break;
    case '(':
      addChar();
      nextToken = LEFT_PAREN;
      break;
    case ')':
      addChar();
      nextToken = RIGHT_PAREN;
      break;
    case ';':
      addChar();
      nextToken = END_OF_LINE;
      break;
    case '{':
      addChar();
      nextToken = CODE_BLOCK_START;
      break;
    case '}':
      addChar();
      nextToken = CODE_BLOCK_END;
      break;    
    // H is keyword for while
    case 'H':
      addChar();
      nextToken = WHILE;
      break;
    // N is keyword for If
    case 'N':
      addChar();
      nextToken = IF;
      break;
    // T is keyword for Else
    case 'T':
      addChar();
      nextToken = ELSE;
      break;
    case ' ':
      addChar();
      nextToken = SPACE;
      break;
    // R is keyword for variable
    case 'R':
      addChar();
      nextToken = VARIABLE;
      break;
      
    //Q - one byte
    case 'Q':
      addChar();
      nextToken = ONE;
      break;
    //W - two byte, 
    case 'W':
      addChar();
      nextToken = TWO;
      break;
    //P - 4 byte, 
    case 'P':
      addChar();
      nextToken = FOUR;
      break;
    //A - 8 byte, 
    case 'A':
      addChar();
      nextToken = EIGHT;
      break;
    //EOF
    default:
      addChar();
      nextToken = EOF;
      break;
    }
    return nextToken;
  }

//Check length of lexemes
  void addChar(void){
    if (lexLen <= 98){
            lexeme[lexLen++] = nextChar;
            lexeme[lexLen] = '\0';
        }
        else
            printf("ERROR - Lexeme is too long\n");
    }

  //Syntax Analyzer
  void Parser(void){
    tok_index = 0;
    synerr_counter = 0;
    //Check BEGIN code
    if (tok[0] == BEGIN_CODE){
      tok_index++;
    }
    else{
      synerr_counter++;
      printf("SYNTAX ERROR: Missing BEGIN_CODE\n");
    }
    //while token != end and EOF call check function 
    while (tok[tok_index] != END_CODE && tok[tok_index] != EOF)
    {
      check();
    }
    if (tok[(tok_index)] != END_CODE && tok[(tok_index - 1)] != END_CODE)
    {
      synerr_counter++;
      printf("SYNTAX ERROR: Missing END_CODE\n");
    }
    if (synerr_counter == 0){
      printf("NO ERROR FOUND!!!!\n");
    }
  }

  //check function Check syntax and return type of error
  void check() {
    if (tok[tok_index] == IDENT) //Check for syntax of open code block {
    {
      tok_index++;
      if (tok[tok_index] == ASSIGN_OP) {
        tok_index++;
        //If correct expression then pass
        if (expr()){ 
          tok_index++;
          if (tok[tok_index] == END_OF_LINE){ // Check for ;
            tok_index++;
          }
          else{
            printf("Missing ; token %d\n", tok[tok_index]);
            synerr_counter++;
            tok_index++;
          }
        }
        else if (nextToken == RIGHT_PAREN) // Check for syntax of  (
        {
          tok_index++;
          if (expr())
          {
            tok_index++;
            if (nextToken == LEFT_PAREN) // Check for )
            {
              tok_index++;
              if (nextToken == END_OF_LINE) // Check for ;
              {
                tok_index++;
              }
              else
              {
                printf("SYNTAX ERROR: Missing ; token %d\n", tok[tok_index]);
                synerr_counter++;
                tok_index++;
              }
            }
            else
            {
              //Check for }
              printf("SYNTAX ERROR: Missing ) token %d\n", tok[tok_index]);  
              synerr_counter++;
              tok_index++;
            }
          }
          else
          {
            printf("Syntax error in arithemetic expression\n");
            synerr_counter++;
            tok_index++;
          }
        }
        else
        {
          synerr_counter++;
          check();
        }
      }
      else
      {
        printf("SYNTAX ERROR: Missing assignemnt, token %d\n", tok[tok_index]);
        synerr_counter++;
        tok_index++;
      }
    }
  
// Check for syntax of variable declaration
    else if (tok[tok_index] == VARIABLE) 
    {
      tok_index++;
      if (tok[tok_index] == IDENT){
        tok_index++;
        if (tok[tok_index] == END_OF_LINE){
          tok_index++;
        }  
        else{
          printf("SYNTAX ERROR: Missing ; token %d\n", tok[tok_index]);
          synerr_counter++;
          tok_index++;
        }
      }
      else{
        printf("SYNTAX ERROR: Missing identifier, token %d\n", tok[tok_index]);
        synerr_counter++;
        tok_index++;
      }
    }

//Check syntax for while statement
    else if (tok[tok_index] == WHILE)
    {
      tok_index++;
      if (tok[tok_index] == LEFT_PAREN){
        tok_index++;
        if (tok[tok_index] == IDENT){
          tok_index++;
          if(tok[tok_index] == EQUAL_TO ||
            tok[tok_index] == NOT_EQUAL_TO ||
            tok[tok_index] == LESS_THAN ||
            tok[tok_index] == LESS_THAN_EQUAL ||
            tok[tok_index] == GREATER_THAN ||
            tok[tok_index] == GREATER_THAN_EQUAL)
          {
            tok_index++;
            if (expr())
            {
              tok_index++;
              if (tok[tok_index] == RIGHT_PAREN){
                tok_index++;
                if (tok[tok_index] == CODE_BLOCK_START) {
                  tok_index++;
                  check();
                  if (tok[tok_index] == CODE_BLOCK_END){
                    tok_index++;
                  }
                  else if(tok[tok_index] == IDENT ||
                          tok[tok_index] == WHILE ||
                          tok[tok_index] == IF ||
                          tok[tok_index] == VARIABLE)
                  {
                    check();
                    while(tok[tok_index] == IDENT ||
                          tok[tok_index] == WHILE ||
                          tok[tok_index] == IF ||
                          tok[tok_index] == VARIABLE){
                    check();
                    }
                    if (tok[tok_index] == CODE_BLOCK_END){
                      tok_index++;
                    }
                    else {
                      printf("SYNTAX ERROR: Missing ) token %d\n", tok[tok_index]);
                      synerr_counter++;
                      tok_index++;
                    }
                  }
                  else{
                    printf("SYNTAX ERROR: Missing ) token %d\n", tok[tok_index]);
                    synerr_counter++;
                    tok_index++;
                  }
                }
                else{
                  printf("SYNTAX ERROR: Missing ( on token %d\n", tok[tok_index]);
                  synerr_counter++;
                  tok_index++;
                }
              }
              else{
                printf("SYNTAX ERROR: Missing ) token %d\n", tok[tok_index]);
                synerr_counter++;
                tok_index++;
              }
            }
            else{
              printf("SYNTAX ERROR: Incorrect arithmetic expression, token %d\n", tok[tok_index]);
              synerr_counter++;
              tok_index++;
            }
          }
          else{
            printf("SYNTAX ERROR: Missing boolean operator, token %d\n", tok[tok_index]);
            synerr_counter++;
            tok_index++;
          }
        }
        else{
          printf("SYNTAX ERROR: Missing identifier for while expression,  token %d\n", tok[tok_index]);
          synerr_counter++;
          tok_index++;
        }
      }
      else
      {
        printf("SYNTAX ERROR: Missing ( for while expression, token %d\n", tok[tok_index]);
        synerr_counter++;
        tok_index++;
      }
    }
      
// Check syntax of if statement
    else if (tok[tok_index] == IF)
    {
      tok_index++;
      if (tok[tok_index] == LEFT_PAREN)
      {
        tok_index++;
        if (tok[tok_index] == IDENT)
      {
            tok_index++;
            if(tok[tok_index] == EQUAL_TO ||
              tok[tok_index] == NOT_EQUAL_TO ||
              tok[tok_index] == LESS_THAN ||
              tok[tok_index] == LESS_THAN_EQUAL ||
              tok[tok_index] == GREATER_THAN ||
              tok[tok_index] == GREATER_THAN_EQUAL)
            {
              tok_index++;
              if (expr()) //Check the right side of the expression
              {
                tok_index++;
                  if (tok[tok_index] == RIGHT_PAREN)
                  {
                    tok_index++;
                    if (tok[tok_index] == CODE_BLOCK_START)
                    {
                      tok_index++;
                      check();
                      if (tok[tok_index] == CODE_BLOCK_END)
                      {
                        tok_index++;
                        if (tok[tok_index] == ELSE)
                        {
                          tok_index++;
                          if (tok[tok_index] == CODE_BLOCK_START)
                          {
                            tok_index++;
                            check();
                            if (tok[tok_index] == CODE_BLOCK_END)
                            {
                              tok_index++;
                            }
                            else if(tok[tok_index] == IDENT ||
                                  tok[tok_index] == WHILE ||
                                  tok[tok_index] == IF ||
                                  tok[tok_index] == VARIABLE)
                            {
                              check();
                              while(tok[tok_index] == IDENT ||
                                tok[tok_index] == WHILE ||
                                tok[tok_index] == IF ||
                                tok[tok_index] == VARIABLE)
                              {
                                check();
                              }
                              if (tok[tok_index] == CODE_BLOCK_END)
                              {
                                tok_index++;
                              }
                              else
                              {
                                printf("SYNTAX ERROR: Missing ) on else statement on token %d\n", tok[tok_index]);
                                synerr_counter++;
                                tok_index++;
                              }
                            }
                            else
                            {
                              printf("SYNTAX ERROR: Missing ) on else statement on token %d\n", tok[tok_index]);
                              synerr_counter++;
                              tok_index++;
                            }
                          }
                          else
                          {
                            printf("SYNTAX ERROR: Missing ( on else statement on token %d\n", tok[tok_index]);
                            synerr_counter++;
                            tok_index++;
                          }
                        }
                      }
                      else if(tok[tok_index] == IDENT ||
                            tok[tok_index] == WHILE ||
                            tok[tok_index] == IF ||
                            tok[tok_index] == VARIABLE)
                      {
                        check();
                        while(tok[tok_index] == IDENT ||
                            tok[tok_index] == WHILE ||
                            tok[tok_index] == IF ||
                            tok[tok_index] == VARIABLE)
                        {
                          check();
                        }
                        if (tok[tok_index] == CODE_BLOCK_END)
                        {
                          tok_index++;
                          if (tok[tok_index] == ELSE)
                          {
                            tok_index++;
                            if (tok[tok_index] == CODE_BLOCK_START)
                            {
                              tok_index++;
                              check();
                              if (tok[tok_index] == CODE_BLOCK_END)
                              {
                                tok_index++;
                              }
                              else if(tok[tok_index] == IDENT ||
                                    tok[tok_index] == WHILE ||
                                    tok[tok_index] == IF ||
                                    tok[tok_index] == VARIABLE)
                              {
                                check();
                                while(tok[tok_index] == IDENT ||
                                    tok[tok_index] == WHILE ||
                                    tok[tok_index] == IF ||
                                    tok[tok_index] == VARIABLE)
                                {
                                  check();
                                }
                                if (tok[tok_index] == CODE_BLOCK_END)
                                {
                                  tok_index++;
                                }
                                else
                                {
                                  printf("SYNTAX ERROR: Missing ) on else statement, token %d\n", tok[tok_index]);
                                  synerr_counter++;
                                  tok_index++;
                                }
                              }
                              else
                              {
                                printf("SYNTAX ERROR: Missing ) on else statement, token %d\n", tok[tok_index]);
                                synerr_counter++;
                                tok_index++;
                              }
                            }
                            else
                            {
                              printf("SYNTAX ERROR: Missing ( on else statement, token %d\n", tok[tok_index]);
                              synerr_counter++;
                              tok_index++;
                            }
                          }
                        }
                        else
                        {
                          printf("SYNTAX ERROR: Missing ) on else statement, token %d\n", tok[tok_index]);
                          synerr_counter++;
                          tok_index++;
                        }
                      }
                      else if (tok[tok_index] == ELSE)
                      {
                        tok_index++;
                        if (tok[tok_index] == CODE_BLOCK_START)
                        {
                          tok_index++;
                          check();
                          if (tok[tok_index] == CODE_BLOCK_END)
                          {
                            tok_index++;
                          }
                          else if(tok[tok_index] == IDENT ||
                                  tok[tok_index] == WHILE ||
                                  tok[tok_index] == IF ||
                                  tok[tok_index] == VARIABLE)
                          {
                            check();
                            while(tok[tok_index] == IDENT ||
                                  tok[tok_index] == WHILE ||
                                  tok[tok_index] == IF ||
                                  tok[tok_index] == VARIABLE)
                            {
                              check();
                            }
                            if (tok[tok_index] == CODE_BLOCK_END)
                            {
                              tok_index++;
                            }
                            else
                            {
                              printf("SYNTAX ERROR: Missing ) on else statement, token %d\n", tok[tok_index]);
                              synerr_counter++;
                              tok_index++;
                            }
                          }
                          else
                          {
                            printf("SYNTAX ERROR: Missing ) on else statement, token %d\n", tok[tok_index]);
                            synerr_counter++;
                            tok_index++;
                          }
                        }
                      else
                      {
                        printf("SYNTAX ERROR: Missing ( on else statement, token %d\n", tok[tok_index]);
                        synerr_counter++;
                        tok_index++;
                      }
                    }
                    else
                    {
                      printf("SYNTAX ERROR: Missing ) for if statement, token %d\n", tok[tok_index]);
                      synerr_counter++;
                      tok_index++;
                    }
                  }
                  else
                  {
                    printf("SYNTAX ERROR: Missing ( for if statement, token %d\n", tok[tok_index]);
                    synerr_counter++;
                    tok_index++;
                  }
                }
                else
                {
                  printf("SYNTAX ERROR: Missing ) for if statement, on token %d\n", tok[tok_index]);
                  synerr_counter++;
                  tok_index++;
                }
              }
              else
              {
                printf("SYNTAX ERROR: Incorrect arithmetic for bool expression, token %d\n", tok[tok_index]);
                synerr_counter++;
                tok_index++;
              }
            }
            else
            {
              printf("SYNTAX ERROR: Missing statement for bool expression, token %d\n", tok[tok_index]);
              synerr_counter++;
              tok_index++;
            }
          }
          else
          {
            printf("SYNTAX ERROR: Missing identifier for bool expression, token %d\n", tok[tok_index]);
            synerr_counter++;
            tok_index++;
          }
        }
            else
            {
                printf("SYNTAX ERROR: Missing ( for bool expression, token %d\n", tok[tok_index]);
                synerr_counter++;
                tok_index++;
            }
        }
        else{
            printf("SYNTAX ERROR: Invalid statement, token %d\n", tok[tok_index]);
            synerr_counter++;
            tok_index++;
        }
    }

//Check for booblen expr
    bool expr(){
        if (tok[tok_index] == IDENT){
            if (tok[(tok_index + 1)] == RIGHT_PAREN && tok[tok_index + 2] == CODE_BLOCK_START)
            {
                return true;
            }
            else if (tok[(tok_index + 1)] == END_OF_LINE)
            {
                return true;
            }
            else if (tok[tok_index + 1] == RIGHT_PAREN)
            {
                return true;
            }
            else if (tok[tok_index + 1] == RIGHT_PAREN && tok[tok_index + 2] == END_OF_LINE)
            {
                return true;
            }
            tok_index++;
            if (tok[tok_index] == ADD_OP ||
                tok[tok_index] == SUB_OP ||
                tok[tok_index] == MULT_OP ||
                tok[tok_index] == DIV_OP)
            {
              tok_index++;
              if (expr())
              {
                return true;
              }
              else
              {
                    printf("SYNTAX ERROR: Identifier or number expected, token %d\n", tok[tok_index]);
                    synerr_counter++;
                    return false;
                }
            }
            else
            {
                printf("SYNTAX ERROR: Arithmetic operator expected, on token %d\n", tok[tok_index]);
                synerr_counter++;
                return false;
            }
        }
        else if (tok[tok_index] == ONE ||
                 tok[tok_index] == TWO ||
                 tok[tok_index] == FOUR ||
                 tok[tok_index] == EIGHT)
        {
            if (tok[(tok_index + 1)] == RIGHT_PAREN && tok[tok_index + 2] == CODE_BLOCK_START)
            {
                return true;
            }
            else if (tok[(tok_index + 1)] == END_OF_LINE)
            {
                return true;
            }
            else if (tok[tok_index + 1] == RIGHT_PAREN)
            {
                return true;
            }
            else if (tok[tok_index + 1] == RIGHT_PAREN && tok[tok_index + 2] == END_OF_LINE)
            {
                return true;
            }
            tok_index++;
            if (tok[tok_index] == ADD_OP ||
                tok[tok_index] == SUB_OP ||
                tok[tok_index] == MULT_OP ||
                tok[tok_index] == DIV_OP)
            {
                tok_index++;

                if (expr())
                {
                    return true;
                }
                else
                {
                    printf("SYNTAX ERROR: Identifier or number expected, token %d\n", tok[tok_index]);
                    synerr_counter++;
                    return false;
                }
            }
            else if (tok[tok_index] != END_OF_LINE)
            {
                printf("SYNTAX ERROR: Missing ; token %d\n", tok[tok_index]);
                synerr_counter++;
                return false;
            }
            else
            {
                printf("SYNTAX ERROR: Arithmetic operator expected, token %d\n", tok[tok_index]);
                synerr_counter++;
                return false;
            }
        }
        else if (tok[tok_index] == LEFT_PAREN)
        {
            tok_index++;
            if (expr())
            {
                tok_index++;
                if (tok[tok_index] == RIGHT_PAREN && (tok[tok_index + 1] == END_OF_LINE))
                {

                    return true;
                }
                else if (tok[tok_index] == ADD_OP ||
                tok[tok_index] == SUB_OP ||
                tok[tok_index] == MULT_OP ||
                tok[tok_index] == DIV_OP)
                {
                    tok_index += 2;
                    if (expr())
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                printf("SYNTAX ERROR: Identifier or number expected, token %d\n", tok[tok_index]);
                synerr_counter++;
                return false;
            }
        }
        else
        {
            synerr_counter++;
            return false;
        }
    }

    //Function to check next input and specify its group
    void getChar(void){
      if ((nextChar = fgetc(in_fp)) != EOF){
        if (islower(nextChar) && isalpha(nextChar) &&
          nextChar != 'H' && nextChar != 'I' && nextChar != 'E' && nextChar != 'R' &&
          nextChar != 'Q' && nextChar != 'W' && nextChar != 'P' && nextChar != 'A')
          charClass = LETTER;
        else if (nextChar == '_')
          charClass = UNDERSCORE;
        else if (nextChar == '$')
          charClass = DOLLA;
        else if (isdigit(nextChar))
          charClass = DIGIT;
        else if (nextChar == 'H'){
          charClass = WHILE;}
        else if (nextChar == 'I') {
          charClass = IF;}
        else if (nextChar == 'E') {
          charClass = ELSE;}
        else if (nextChar == 'R') {
          charClass = VARIABLE;}
        else if (nextChar == 'Q') {
          charClass = ONE;}
        else if (nextChar == 'W') {
          charClass = TWO;}
        else if (nextChar == 'P') {
          charClass = FOUR;}
        else if (nextChar == 'A') {
          charClass = EIGHT;}
        else if (isupper(nextChar)) {
          charClass = ERROR; }
        else
          charClass = ERROR;
        }
        else
          charClass = EOF;
    }
    void getNonBlank(void){
      while (isspace(nextChar))
        getChar();
    }
    //Analyzer for keyword and expression
    int lex(void){
        lexLen = 0;
        getNonBlank();
        switch (charClass)
        {

        case UNDERSCORE:
            addChar();
            getChar();
            break;
          
        case DOLLA:
            addChar();
            getChar();
            break;

        ///[a-z_]{6,8}
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER ||
                   charClass == UNDERSCORE && (lexLen >= 6 || lexLen <= 8)) {
                addChar();
                getChar();}
            if (!strcmp(lexeme, "begin")) {
                nextToken = BEGIN_CODE;}
            else if (!strcmp(lexeme, "end")) {
              nextToken = END_CODE;}
            else if (lexLen >= 6 && lexLen <= 8)
              nextToken = IDENT;
            else {
              while (charClass == ERROR){
                addChar();
                getChar();}
              nextToken = ERROR;}
            break;
          
        //[0-9]$Q, [0-9]$W, [0-9]$P, [0-9]$A
        case DIGIT: 
            addChar();
            getChar();
            while (charClass == DIGIT || charClass == DOLLA) {
                addChar();
                getChar();
            }
            if (charClass == ONE) {
                nextToken = ONE;}
            else if (charClass == TWO) {
                nextToken = TWO;}
            else if (charClass == FOUR) {
                nextToken = FOUR;}
            else if (charClass == EIGHT) {
                nextToken = EIGHT;}
            addChar();
            getChar();
            break;
        case ERROR:
            lookup(nextChar);
            getChar();
            break;
        case WHILE:
            nextToken = WHILE;
            addChar();
            getChar();
            break;
        case IF:
            addChar();
            getChar();
            nextToken = IF;
            break;
        case ELSE:
            addChar();
            getChar();
            nextToken = ELSE;
            break;
        case VARIABLE:
            addChar();
            getChar();
            nextToken = VARIABLE;
            break;
            /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = '\0';
            break;
        }
        printf("Next token is: %d; Next lexeme is %s\n",
          nextToken, lexeme);
        return nextToken;
    }
};

/******************************************************/
/* main function */
int main(void)
{
  //Call lexer
    Lexer lex_analyzer;
    int lex_counter = 0;

    //Read input from file
    if ((lex_analyzer.in_fp = fopen("correct1.txt", "r")) == NULL)
        printf("ERROR - File not found!!!\n");
    else{
        lex_analyzer.getChar();
        lex_analyzer.lex();
        lex_analyzer.tok[lex_counter] = lex_analyzer.nextToken;
        lex_counter++;
        do{
            lex_analyzer.lex();
            lex_analyzer.tok[lex_counter] = lex_analyzer.nextToken;
            lex_counter++;
        }while (lex_analyzer.nextToken != EOF);
      // Call parser check for syntax
        lex_analyzer.Parser();
    }
    return 0;
}
