import os
import math 
import time

# http://stackoverflow.com/questions/2467590/dynamically-evaluating-simple-boolean-logic-in-python

app_name = "ConsoleV2PC++.exe"

func = "((x1+x2)*(x3+x4))"
param_prefix = "-x"
params_count = 4

str_to_token = {'1':True,
                '0':False,
                '*':lambda left, right: left and right,
                '+':lambda left, right: left or right,
                '(':'(',
                ')':')'}

empty_res = True


def create_token_lst(s, str_to_token=str_to_token):
    """create token list:
    'True or False' -> [True, lambda..., False]"""
    s = s.replace('(', ' ( ')
    s = s.replace(')', ' ) ')

    return [str_to_token[it] for it in s.split()]


def find(lst, what, start=0):
    return [i for i,it in enumerate(lst) if it == what and i >= start]


def parens(token_lst):
    """returns:
        (bool)parens_exist, left_paren_pos, right_paren_pos
    """
    left_lst = find(token_lst, '(')

    if not left_lst:
        return False, -1, -1

    left = left_lst[-1]

    #can not occur earlier, hence there are args and op.
    right = find(token_lst, ')', left + 4)[0]

    return True, left, right


def bool_eval(token_lst):
    """token_lst has length 3 and format: [left_arg, operator, right_arg]
    operator(left_arg, right_arg) is returned"""
    return token_lst[1](token_lst[0], token_lst[2])


def formatted_bool_eval(token_lst, empty_res=empty_res):
    """eval a formatted (i.e. of the form 'ToFa(ToF)') string"""
    if not token_lst:
        return empty_res

    if len(token_lst) == 1:
        return token_lst[0]

    has_parens, l_paren, r_paren = parens(token_lst)

    if not has_parens:
        return bool_eval(token_lst)

    token_lst[l_paren:r_paren + 1] = [bool_eval(token_lst[l_paren+1:r_paren])]

    return formatted_bool_eval(token_lst, bool_eval)


def nested_bool_eval(s):
    """The actual 'eval' routine,
    if 's' is empty, 'True' is returned,
    otherwise 's' is evaluated according to parentheses nesting.
    The format assumed:
        [1] 'LEFT OPERATOR RIGHT',
        where LEFT and RIGHT are either:
                True or False or '(' [1] ')' (subexpression in parentheses)
    """
    return formatted_bool_eval(create_token_lst(s))

def _mkdir(newdir):
    """works the way a good mkdir should :)
        - already exists, silently complete
        - regular file in the way, raise an exception
        - parent directory(ies) does not exist, make them as well
    """
    if os.path.isdir(newdir):
        pass
    elif os.path.isfile(newdir):
        raise OSError("a file with the same name as the desired " \
                      "dir, '%s', already exists." % newdir)
    else:
        head, tail = os.path.split(newdir)
        if head and not os.path.isdir(head):
            _mkdir(head)
        #print "_mkdir " + repr(newdir)
        if tail:
            os.mkdir(newdir)

def report(s, dirpath) :
    f = open(dirpath + "\\report.txt", "w")
    f.write(s + "\n")
    print(s + "\n")
    f.close()	

def invoke() :
	max = math.pow(2, params_count)
	for i in range(0, int(max)) :
		s = ""
		expr = func
		x = bin(i)[2:].zfill(params_count)
		s += app_name + " -f " + func
		for k in range(1, params_count + 1) :
			s += " -x" + str(k) + " " + x[k - 1]
			expr = expr.replace("x" + str(k), x[k - 1])
		dir = "c:\\prove\\prova_" + str(i)
		_mkdir(dir)
		s += " -o " + dir
		s += " -e 16"
		report(s, dir)
		expr = expr.replace("*", " * ").replace("+"," + ")
		report("eseguo " + s + " risultato attesto di " + expr + " " + str(nested_bool_eval(expr)), dir)
		os.system("start /wait " + s)
		time.sleep(1)


invoke()

