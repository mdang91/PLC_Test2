a.Define the rules for recognizing all lexemes as their proper token.
Operation symbols:
Addition: +
Subtraction: -
Multiplication: *
Division: /
Modulo: %
Less than: <
Greater than: >
Less than or equal: !
Greater than or equal: ^
Equal: @
Not equal: #
Assignment: =
Parenthesis open: (
Parenthesis close: )

Variable Name: 
Variable names rule: ^[a-zA-Z_]{6,8} (6-8 letters only word may contain undersore)

Keyword
Variable type: R
While statement: H
If statement: N
Else statement: T

Different int sizes:
1 byte: [0-9]$Q
2 byte: [0-9]$W
4 byte: [0-9]$P
8 byte: [0-9]$A
=================================================================
b.Define production rules for implementing the mathematical syntax of operators and operands, loops, variable declaration, selection statements.

<prog> -> begin <stmt_list>
<stmt_list> -> <stmt> <stmt_list> | end
<stmt> -> <if_stmt> | <while_stmt> | <assignment> | <declare>
<declare> -> id
<if_stmt> -> N  (  <bool_exp>  )  <block>  T <block> 
<while_stmt> -> H  (  <bool_exp>  )  <block>
<assignment> -> id = <expression>

<term> —> <factor> { ( + | - ) <factor> }
<expression> —> <term> { (  *  | /  | % ) <term> }

<factor> —>   id  | int_lit  | ( <expression> )
<bool_exp> —> <bel> { ( @ | # ) <bel> }
<bel> —> <bexpr> { ( < | > | ! | ^ ) <bexpr> }
<bexpr> —> <bterm>  {  ( * | / | % ) <bterm> }
<bterm> —> <bfactor> { ( + | - ) <bfactor> }
<bfactor> —> id | int_lit | bool_lit | (  <bexpr> )

=================================================================
c.Show the language conforms to the standard of an LL Grammar.
=> In the rule in b, there is no rule that exists in the same line. Thus there is no left-hand recursion.
=> Test Pairwise Disjointness:
FIRST (prog) -  {bengin}
FIRST (stmt_list) - {N, H, id, R, {}
FIRST (expression) - {id, int_lit, ( }
FIRST (bool_exp) - {id, int_lit, bool_lit, ( }
=> The rule passes the pairwise disjointness test. Therefore, the language conforms to the standards of an LL Grammar.

=================================================================
d.Not ambiguous grammar.
=> The program is not ambiguous because there is only one way to generate the rule. The program can only start with Begin and it has to go to the first rule. Every possible statement is unique and cannot form a duplicate string.

=================================================================
e.Write a program that process all lexemes in a file by recognizing all tokens in a file, and produces a list of those tokens in order.
=> Implemented as class Lexer in the program.

=================================================================
f.Write a program or an extension to the above program.
=> Implemented as class Parser in the program.

=================================================================
g.Check Error
=> File lexerror.txt is lexical error
begin //correct
R c_c_c_c_c_c_c; //wrong - more than 8 characters
R ddddddd; //correct
R eeeeee; //correct
R ggggg; // wrong - need to have at least 6 characters
r aaaaaa; // wrong - r not define in the language - suppose to be R
if (ccccccc @ 100){ //if not a keyword in the language N for if, 100 is not correct suppose 100${Q,W,P,A}
  bbbbbb = 1 + 2; //wrong - suppose 1$A + 2$A
}
else { // else not a keyword suppose T
  bbbbbb = 3$A + 4$A;
}
en //wrong - end

=> File synerror.txt is syntax error
begin //correct
R t_testtt; //correct
R ddddddd; //correct
aaaaaa; //wrong - missing keyword to declare variable
N (){ // wrong - missing boolean expr in if statement
  bbbbbb = 1$A + 2$ // wrong - missing semicolon
 // missing close code block
T{
  bbbbbb = 3$A + 4$A; // correct
}
H(){ // wrong - missing boolean expr for while block
  nnnnnn = nnnnnn + 1$A;; // wrong - double semicolon
}
end

=> File test3.txt, test4.txt are correct

=================================================================

h.Parse Table
1. Input: begin n ( id < id ) { h ( id # id ) { id = id ; } } end
2. Input: begin n ( id < id ) { id = id ; } t { id = ( id + id ) ; } end
3. Input: begin n ( id < id ) { h ( id > id ) ) { id = ( id + id ) ; } } end // wrong since there is extra ) in the input
4. Input: begin n ( id < id ) { id / id ; ) t { id = ( id + id ) ; } end // wrong because it should be assignment inside if 

