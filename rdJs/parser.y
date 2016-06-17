
%start program

%%

program : blocks
        ;

blocks  : block
        | blocks block
        ;

/* Each block can be an assignment or a variable declaration */
block   : decl END
        | assign END
        | error END
        ;

/* for definition
* e.g. for (12; 2) { scope }
*/
for : FOR L_PARENT forvalue SEMI_COLON forvalue R_PARENT L_BRACES forscope R_BRACES

forscope : declaration
         | for

forvalue : INT
         | DOUBLE
         | ID
/* end for */

/* Variable declaration region.
 * e.g. var umnome, segundonome;
 */
declaration : VAR varlist SEMI_COLON

varlist : ID
        | varlist COMMA ID
/* end declaration */

/* Variable assignment region.
* e.g. umnome = 1; segundonome = umnome;
*/
assignment : ID ASSIGN value SEMI_COLON

value : INT
      | DOUBLE
      | BOOL
      | ID
/* end assignment */

param : value
      | param COMMA value

func : FUNC ID L_PARENT param R_PARENT L_BRACES R_BRACES
%%
