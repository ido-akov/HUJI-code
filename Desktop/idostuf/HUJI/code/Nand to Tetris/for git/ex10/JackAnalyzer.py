import sys
import os
from JackTokenizer import JackTokenizer
import grammar
from lxml import etree

# constants
JACK = ".jack"
WRITE = 'wb'
XML = ".xml"

# global variables
output_XML_file = None
# token generator
tokens = None


def write_tokens(num, parent):
    """
    write tokens to XML file
    :param num: of tokens
    :param parent: for XML for XML
    """
    [write_token(parent) for i in range(num)]


def compile_class():
    """
    :param parent: for XML for XML
    """
    root = etree.Element(grammar.CLASS)
    # class className {
    write_tokens(3, root)
    next_token = tokens.peek(1)
    # classVarDec*
    if next_token.value in grammar.CLASS_VAR_DECS:
        compile_zero_or_more(root, next_token, grammar.CLASS_VAR_DECS,
                             compile_class_var_dec)
        next_token = tokens.peek(1)
    # subroutineDec*
    if next_token.value in grammar.SUB_ROUTINE_DECS:
        compile_zero_or_more(root, next_token, grammar.SUB_ROUTINE_DECS,
                             compile_sub_routine)
    # '}'
    write_token(root)
    return root

def compile_zero_or_more(parent, next_token, char_set, compile_func,
                         section_name=None):
    """
    helper function for compiling (smallersection*)
    :param parent: for XML current parent
    :param next_token: in jack file next_token to write
    :param section_name: current section
    :param char_set: set of chars which a smallersection must begin with
    :param compile_func: function to compile a smallersection
    or every iteration

    """
    if section_name:
        #write only one header: statements
        node = etree.SubElement(parent, section_name)
        while next_token.value in char_set:
            compile_func(next_token, node)
            next_token = tokens.peek(1)
    else:
        # multiple header/closers for each smaller section:
        #classVarDec, varDec, subRoutineDec
        while next_token.value in char_set:
            compile_func(next_token, parent)
            next_token = tokens.peek(1)


def compile_class_var_dec(next_token, parent):
    """
    :param next_token: in jack file
    :param parent: for XML
    """
    node = etree.SubElement(parent, grammar.CLASS_VAR_DEC)

    # ('static' | 'field' ) type varName (',' varName)*
    while next_token.value != grammar.SEMICOLON:
        write_token(node)
        next_token = tokens.peek(1)
    # ';'
    write_token(node)


def compile_sub_routine(next_token, parent):
    """
    :param next_token: in jack file
    :param parent: for XML
    """
    # ('constructor' | 'function' | 'method') ('void' | type) subroutineName
    node = etree.SubElement(parent, grammar.SUB_ROUTINE_DEC)

    while next_token.value != grammar.RIGHT_PARENS:
        write_token(node)
        next_token = tokens.peek(1)
    # '('
    write_token(node)
    # parameterList
    compile_parameter_list(tokens.peek(1), node)
    # ')'
    write_token(node)
    # OPENS the sub routine body
    compile_sub_routine_body(node)


def compile_sub_routine_body(parent):
    """
    :param parent: for XML
    """
    node = etree.SubElement(parent, grammar.SUB_ROUTINE_BODY)
    # '{'
    write_token(node)
    # varDec*
    next_token = tokens.peek(1)
    if next_token.value == grammar.VAR:
        compile_zero_or_more(node, next_token, {grammar.VAR},
                             compile_var_dec)
    # statements
    compile_zero_or_more(node, tokens.peek(1), grammar.STATEMENT_SET,
                         compile_statements, grammar.STATEMENTS)
    # '}'
    write_token(node)
    # closes the sub routine body


def write_token(parent):
    """
    write token to XML file
    :param parent: for XML
    """
    cur_token = next(tokens)
    node = node = etree.SubElement(parent, cur_token.type)
    node.text = cur_token.value


def compile_parameter_list(next_token, parent):
    """
    compiles a (possibly empty) parameter list, not including the enclosing parens.
    :param next_token: in jack file
    :param parent: for XML
    """
    node = etree.SubElement(parent, grammar.PARAMETER_LIST)
    # ( (type varName) (',' type varName)*)?
    while next_token.value != grammar.LEFT_PARENS:
        write_token(node)
        next_token = tokens.peek(1)


def compile_var_dec(next_token, parent):
    """
    compiles a var declaration.
    :param next_token: in jack file
    :param parent: for XML
    """
    node = etree.SubElement(parent, grammar.VAR_DEC)
    while next_token.value != grammar.SEMICOLON:
        write_token(node)
        next_token = tokens.peek(1)
    # ';'
    write_token(node)


def compile_statements(next_token, parent):
    """
    compile one of multiple statements
    :param next_token: in jack file
    :param parent: for XML
    """
    if next_token.value == grammar.LET:
        compile_let(parent)
    elif next_token.value == grammar.IF:
        compile_if(parent)
    elif next_token.value == grammar.WHILE:
        compile_while(parent)
    elif next_token.value == grammar.DO:
        compile_do(next_token, parent)
    elif next_token.value == grammar.RETURN:
        compile_return(parent)


def compile_do(next_token, parent):
    """
    compile a do statement
    :param next_token: in jack file
    :param parent: for XML
    """
    node = etree.SubElement(parent, grammar.DO_STATEMENT)
    # 'do' ((className|varName)'.')? subroutineName '('
    # expressionList ')'
    compile_sub_routine_call(node, next_token)
    # ';'
    write_token(node)


def compile_sub_routine_call(parent, next_token):
    """
    compile a subroutine call
    :param parent: for XML
    :param next_token: in jack file
    """
    # 'do' ((className|varName)'.')? subroutineName '(' expressionList ')'
    while next_token.value != grammar.RIGHT_PARENS:
        write_token(parent)
        next_token = tokens.peek(1)
    # '('
    write_token(parent)
    # open expressionList
    node = etree.SubElement(parent, grammar.EXPRESSION_LIST)
    next_token = tokens.peek(1)
    if next_token.value != grammar.LEFT_PARENS:
        # expressionList
        compile_expression_list(node)
    # close expressionList
    # ')'
    write_token(parent)


def compile_let(parent):
    """
    compiles a let statement
    :param parent: for XML
    """

    node = etree.SubElement(parent, grammar.LET_STATEMENT)

    # 'let' varName
    write_tokens(2, node)
    next_token = tokens.peek(1)
    if next_token.value != grammar.EQUALS:
        # '['
        write_token(node)
        # expression
        compile_expression(node)
        # ']'
        write_token(node)
    # '='
    write_token(node)
    # expression
    compile_expression(node)
    # ';'
    write_token(node)


def compile_while(parent):
    """
    Compiles a while statement
    :param parent: for XML
    """
    node = etree.SubElement(parent, grammar.WHILE_STATEMENT)

    # 'while' '('
    write_tokens(2, node)
    # expression
    compile_expression(node)
    # ')' '{'
    write_tokens(2, node)
    # statements
    compile_zero_or_more(node, tokens.peek(1), grammar.STATEMENT_SET,
                         compile_statements, grammar.STATEMENTS)
    # "}"
    write_token(node)


def compile_return(parent):
    """
    compiles a return statement.
    :param parent: for XML
    """
    node = etree.SubElement(parent, grammar.RETURN_STATEMENT)

    # 'return'
    write_token(node)
    next_token = tokens.peek(1)
    if next_token.value != grammar.SEMICOLON:
        # expression?
        compile_expression(node)
    # ';'
    write_token(node)


def compile_if(parent):
    """
    compiles an if statement, possibly
    with a trailing else clause.
    :param parent: for XML
    """
    node = etree.SubElement(parent, grammar.IF_STATEMENT)

    # 'if' '('
    write_tokens(2, node)
    # expression
    compile_expression(node)
    # ')' '{'
    write_tokens(2, node)
    # statements
    compile_zero_or_more(node, tokens.peek(1), grammar.STATEMENT_SET,
                         compile_statements, grammar.STATEMENTS)
    # "}"
    write_token(node)
    next_token = tokens.peek(1)
    if next_token.value == grammar.ELSE:
        # 'else' '{'
        write_tokens(2, node)
        # statements
        compile_zero_or_more(node, tokens.peek(1), grammar.STATEMENT_SET,
                             compile_statements, grammar.STATEMENTS)
        # "}"
        write_token(node)


def compile_expression(parent):
    """
    CompileExpression -- -- compiles an expression.
    :param parent: for XML
    """
    node = etree.SubElement(parent, grammar.EXPRESSION)

    # term
    compile_term(node)
    next_token = tokens.peek(1)
    while next_token.value in grammar.OPS:
        # (op term)*
        write_token(node)
        compile_term(node)
        next_token = tokens.peek(1)


def compile_expression_list(parent):
    """
    compile an expression list within a subroutine call
    :param parent: for XML for XML
    """
    # expression
    compile_expression(parent)
    next_token = tokens.peek(1)
    while next_token.value == grammar.COMMA:
        # (',' expression)* )?
        write_token(parent)
        compile_expression(parent)
        next_token = tokens.peek(1)


def compile_term(parent):
    """
    compile a term
    :param parent: for XML for XML
    """
    node = etree.SubElement(parent, grammar.TERM)

    next_token = tokens.peek(1)
    if next_token.value == grammar.RIGHT_PARENS:
        # '(' expression ')'
        write_token(node)
        compile_expression(node)
        write_token(node)
    elif next_token.value in grammar.UNARY_OPS:
        # unaryOp term
        write_token(node)
        compile_term(node)
    else:
        next_next_token = tokens.peek(2)
        if next_next_token.value == grammar.RIGHT_SQUARE_BRACKET:
            # varName '[' expression ']'
            write_tokens(2, node)
            compile_expression(node)
            write_token(node)
        elif (next_next_token.value == grammar.DOT or
              next_next_token.value == grammar.RIGHT_PARENS):
            # subRoutineCall
            compile_sub_routine_call(node, next_token)
        else:
            # integerConstant | stringConstant | keywordConstant | varName
            write_token(node)


def compilation_engine(jack_file):
    """
    compiler starter
    :param jack_file: file to compile
    """
    global output_XML_file, tokens
    tokens = JackTokenizer(jack_file)
    with open(jack_file[:-5] + XML, WRITE) as output_XML_file:
        root = compile_class()
        data = etree.tostring(root, pretty_print=True)
        output_XML_file.write(data)

def main():
    """
    get path to file/dir as argument, tokenize file if ends with .jack,
     or all such files in dir
    """
    #
    path = sys.argv[1]
    # if path points to file with .jack suffix tokenize it
    if os.path.isfile(path) and os.path.splitext(path)[-1] == JACK:
        compilation_engine(path)
    else:
        if os.path.isdir(path):
            # tokenize all files in directory ending with .jack
            [compilation_engine(os.path.join(path, file)) for file in os.listdir(path)
             if file.endswith(JACK)]


if __name__ == "__main__":
    main()
