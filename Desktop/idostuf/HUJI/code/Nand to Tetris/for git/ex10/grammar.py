#syntax analysis constants

KEYWORD = "keyword"
SYMBOL = "symbol"
COMMA = ','
DOT = '.'
EQUALS = '='
INTEGER_CONSTANT = "integerConstant"
STRING_CONSTANT = "stringConstant"
IDENTIFIER = "identifier"
OPS = {'+', '-', '*', '/', '&', '|', '<', '>', '='}
RIGHT_PARENS = '('
LEFT_PARENS = ')'
RIGHT_SQUARE_BRACKET = '['
SEMICOLON = ';'
CLASS = "class"
CLASS_VAR_DEC = 'classVarDec'
CLASS_VAR_DECS = {'static', 'field'}
CLASS_KINDS = CLASS_VAR_DECS
VAR = 'var'
SUB_ROUTINE_KINDS = {"argument", VAR}
SUB_ROUTINE_DEC = 'subroutineDec'
SUB_ROUTINE_DECS = {'constructor', 'function', 'method'}
PARAMETER_LIST = 'parameterList'
VAR_DEC = 'varDec'
SUB_ROUTINE_BODY = 'subroutineBody'
STATEMENTS = 'statements'
STATEMENT_SET = {"if", "do", "while", "let", "return"}
LET_STATEMENT = 'letStatement'
IF_STATEMENT = 'ifStatement'
WHILE_STATEMENT = 'whileStatement'
DO_STATEMENT = 'doStatement'
RETURN_STATEMENT = 'returnStatement'
EXPRESSION = 'expression'
TERM = 'term'
EXPRESSION_LIST = 'expressionList'
LET = 'let'
IF = 'if'
ELSE = 'else'
WHILE = 'while'
DO = 'do'
RETURN = 'return'
UNARY_OPS = {'-', '~'}