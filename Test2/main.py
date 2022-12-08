from __future__ import print_function
import sys

#Token identifier

identifier = ["Op_add", "Op_subtract", "Op_multiply", "Op_divide", "Op_mod", 
    "Op_negate", "Op_not", "Op_less", "Op_lessequal", "Op_greater", "Op_greaterequal",
    "Op_equal", "Op_notequal", "Op_assign", "Op_and", "Op_or", "Keyword_if",
    "Keyword_else", "Keyword_while", "Keyword_for", "Keyword_do", "LeftParen",
    "RightParen", "LeftBrace", "RightBrace", "Semicolon", "Comma", "Identifier",
    "Integer", "Long", "End_of_input", "String"]

tk_Add, tk_Sub, tk_Mul, tk_Div, tk_Mod, tk_Negate, tk_Not, tk_Lss, tk_Leq, tk_Gtr, \
tk_Geq, tk_Eq, tk_Neq, tk_Assign, tk_And, tk_Or, tk_If, tk_Else, tk_While, tk_For,       \
tk_Do, tk_Lparen, tk_Rparen, tk_Lbrace, tk_Rbrace, tk_Semi, tk_Comma, tk_Ident,          \
tk_Integer, tk_Long, tk_EOI, tk_String = range(32)

# Special symbol
ssymbols = { '+': tk_Add, '-': tk_Sub, '*': tk_Mul, '%': tk_Mod, ';': tk_Semi, ',': tk_Comma, '{': tk_Lbrace, '}': tk_Rbrace, '(': tk_Lparen, ')': tk_Rparen  }

key_words = {'icondi': tk_If, 'econdi': tk_Else, 'whlp': tk_While, 'lg': tk_Long, 'flp': tk_For,  'dwhlp': tk_Do, 'numeric': tk_Integer}

string_input = " "    # dummy first char - but it must be a space
num = 0
line_check = 1
input_file = None

# show error and exit
def error(line, col, msg):
    print(line, col, msg)
    exit(1)

# get the next character from the input
def next_ch():
    global string_input, num, line_check

    string_input = input_file.read(1)
    num += 1
    if string_input == '\n':
        line_check += 1
        num = 0
    return string_input

# 'x' - character constants
def char_lit(err_line, err_col):
    n = ord(next_ch())              # skip opening quote
    if string_input == '\'':
        error(err_line, err_col, "empty character constant")
    elif string_input == '\\':
        next_ch()
        if string_input == 'n':
            n = 10
        elif string_input == '\\':
            n = ord('\\')
        else:
            error(err_line, err_col, "unknown escape sequence \\%c" % (string_input))
    if next_ch() != '\'':
        error(err_line, err_col, "multi-character constant")
    next_ch()
    return tk_Integer, err_line, err_col, n

#*** process divide or comments
def div_or_cmt(err_line, err_col):
    if next_ch() != '*':
        return tk_Div, err_line, err_col

    # comment found
    next_ch()
    while True:
        if string_input == '*':
            if next_ch() == '/':
                next_ch()
                return gettok()
        elif len(string_input) == 0:
            error(err_line, err_col, "EOF in comment")
        else:
            next_ch()

#*** "string"
def string_lit(start, err_line, err_col):
    global string_input
    text = ""

    while next_ch() != start:
        if len(string_input) == 0:
            error(err_line, err_col, "EOF while scanning string literal")
        if string_input == '\n':
            error(err_line, err_col, "EOL while scanning string literal")
        if string_input == '\\':
            next_ch()
            if string_input != 'n':
                error(err_line, err_col, "escape sequence unknown \\%c" % string_input)
            string_input = '\n'
        text += string_input

    next_ch()
    return tk_String, err_line, err_col, text

#*** handle identifiers and integers
def ident_or_int(err_line, err_col):
    is_number = True
    text = ""

    while string_input.isalnum() or string_input == '_':
        text += string_input
        if not string_input.isdigit():
            is_number = False
        next_ch()

    if len(text) == 0:
        error(err_line, err_col, "ident_or_int: unrecognized character: (%d) '%c'" % (ord(string_input), string_input))

    if text[0].isdigit():
        if not is_number:
            error(err_line, err_col, "invalid number: %s" % (text))
        n = int(text)
        return tk_Integer, err_line, err_col, n

    if text in key_words:
        return key_words[text], err_line, err_col
    return tk_Ident, err_line, err_col, text

#*** look ahead for '>=', etc.
def follow(expect, ifyes, ifno, err_line, err_col):
    if next_ch() == expect:
        next_ch()
        return ifyes, err_line, err_col

    if ifno == tk_EOI:
        error(err_line, err_col, "follow: unrecognized character: (%d) '%c'" % (ord(string_input), string_input))

    return ifno, err_line, err_col

#*** return the next token type
def getChar():
    while string_input.isspace():
      next_ch()

    err_line = line_check
    err_col  = num

    if len(string_input) == 0:    return tk_EOI, err_line, err_col
    elif string_input == '/':     return div_or_cmt(err_line, err_col)
    elif string_input == '\'':    return char_lit(err_line, err_col)
    elif string_input == '<':     return follow('=', tk_Leq, tk_Lss,    err_line, err_col)
    elif string_input == '>':     return follow('=', tk_Geq, tk_Gtr,    err_line, err_col)
    elif string_input == '=':     return follow('=', tk_Eq,  tk_Assign, err_line, err_col)
    elif string_input == '!':     return follow('=', tk_Neq, tk_Not,    err_line, err_col)
    elif string_input == '&':     return follow('&', tk_And, tk_EOI,    err_line, err_col)
    elif string_input == '|':     return follow('|', tk_Or,  tk_EOI,    err_line, err_col)
    elif string_input == '"':     return string_lit(string_input, err_line, err_col)
    elif string_input in ssymbols:
        sym = ssymbols[string_input]
        next_ch()
        return sym, err_line, err_col
    else: return ident_or_int(err_line, err_col)

      
#####reading from file#####
#### type: python3 main.py <textfile>
input_file = sys.stdin
if len(sys.argv) > 1:
    try:
        input_file = open(sys.argv[1], "r", 2048)
    except IOError as e:
        error(0, 0, "Can't open %s" % sys.argv[1])
################################################################
      
while True:
  
    g = getChar()
    token  = g[0]
    line = g[1]
    col  = g[2]
  
    print("%5d\t%-14s" % (col, identifier[token]), end='')

    if token == tk_Integer:  print("%d" %(g[3]))
    elif token == tk_Ident:  print("%s" %(g[3]))
    elif token == tk_String: print('"%s"' %(g[3]))
    else:                  print("")

    if token == tk_EOI:
        break