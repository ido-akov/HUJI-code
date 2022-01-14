import re
import grammar

# regexes
COMMENT_REGEX = "\\/\\/.*|\\s*\\/[*]{1,2}.*|.*?[*]\\/\\s*"
NON_WHITESPACE_REGEX = "[\S]+"
COMMENT_OPENER_REGEX = "\\s*\\/[*]{1,2}.*"
COMMENT_CLOSER_REGEX = ".*?[*]\\/"
KEYWORD_REGEX = "\\s*(class|constructor|function|method|field|static|var|int|char|boolean|void" \
                "|true|false|null|this|let|do|if|else|while|return)\\s+"
SYMBOL_REGEX = "\\s*(\{|\}|\(|\)|\[|\]|\.|\,|\;|\+|\-|\*|\/|\&|\||\<|\>|\=|\~)\\s*"
INTEGER_CONSTANT_REGEX = "\\s*([0-9][\d]{0,4})\\s*"
STRING_CONSTANT_REGEX = '\\s*"([^"\n]*?)"\\s*'
IDENTIFIER_REGEX = "\\s*([a-zA-Z_][\\w]*)\\s*"

# constants
EMPTY_STRING = ""
READ = 'r'
LT = 'lt;'
GT = 'gt;'
AMP = 'amp;'
AMPERSAND = '&'
MARKUP_DICT = {'<': LT, '>': GT, '&': AMP}
#dict for evaluating Token type according to regex match
TYPE_DICT = {KEYWORD_REGEX: grammar.KEYWORD, SYMBOL_REGEX: grammar.SYMBOL,
             INTEGER_CONSTANT_REGEX: grammar.INTEGER_CONSTANT,
             STRING_CONSTANT_REGEX: grammar.STRING_CONSTANT,
             IDENTIFIER_REGEX: grammar.IDENTIFIER}


class Token:
    """
    class for encapsulation of jack syntax analysis data
    """
    def __init__(self, type, value):
        self.type = type
        self.value = value

    def __repr__(self):
        #when representing Token substitute value if in MARKUP_DICT
        value = AMPERSAND + MARKUP_DICT[self.value] if self.value in MARKUP_DICT\
            else self.value
        return '<{0}> {1} </{0}> '.format(self.type, value)

class JackTokenizer:
    """
    jack file Token generator with peeking capabilities
    """

    def __init__(self, file):
        #Token generator
        self.generator = self.tokenize(file)
        #stack for peeking
        self.pushed_back = []

    def __iter__(self):
        return self

    def __next__(self):
        if self.pushed_back:
            return self.pushed_back.pop()
        else:
            return next(self.generator)

    def push_back(self, element):
        """
        helper function for peeking
        :param element: to peek
        """
        self.pushed_back.append(element)

    def peek(self, k):
        """
        peek at kth element forward
        :param k: number of iterations forward to peek
        :return: kth element forward
        """
        if len(self.pushed_back) >= k:
            # element we want to peek has been pushed back
            index = len(self.pushed_back) - k
            return self.pushed_back[index]
        else:
            # push back as many as necessary to peek kth element
            self.pushed_back = self.pushed_back[::-1]
            r = k - len(self.pushed_back)
            for i in range(r):
                x = next(self.generator)
                self.push_back(x)
            self.pushed_back = self.pushed_back[::-1]
            return self.pushed_back[len(self.pushed_back) - k]

    def is_multi_comment(self, line, multi_comment):

        comment_opener = re.search(COMMENT_OPENER_REGEX, line)
        if comment_opener:
            multi_comment = True
        comment_closer = re.search(COMMENT_CLOSER_REGEX, line)
        if comment_closer:
             multi_comment = False
        return multi_comment

    def has_more(self):
        try:
            self.peek(1)
            return True
        except StopIteration:
            return False

    def tokenize(self, jack_file):
        with open(jack_file, READ) as file:
            multi_comment = False
            for line in file:
                multi_comment = self.is_multi_comment(line, multi_comment)
                line = re.sub(COMMENT_REGEX, EMPTY_STRING, line)
                if multi_comment or not re.search(NON_WHITESPACE_REGEX, line):
                    continue
                while line:
                    for regex in TYPE_DICT:
                        match = re.match(regex, line)
                        if match:
                            yield Token(TYPE_DICT[regex], match.group(1))
                            # look at next chars in line
                            line = line[:match.start()] + line[match.end():]
                            break