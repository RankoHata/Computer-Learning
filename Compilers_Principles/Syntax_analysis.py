"""实现语法分析

文法如下:
    G[E]:
    E -> E+T|T
    T -> T*F|F
    F -> (E)|i

实验文法约定:
    1. 输入文法中无多余空格
    2. 使用字符$(代表空)
    3. 非终结符均为大写字母
    4. 终结符均为ASCII字符
    5. 定义关系仅为->(暂不支持::=)
    6. 默认输入的第一个非终结符为文法开始符号
"""

from copy import deepcopy  # 深拷贝


class Config(object):
    symbol_empty = '$'
    nonterminal_chars = []  # 非终结符
    terminal_chars = []  # 终结符
    production_data = []  # 存储规则数据
    first_set_list = []
    follow_set_list = []
    nullable_set = set()  # Note: 存放的是*可以*推出空符号的非终结符，而非必然推出中介符号的终结符
    old_first_set_list = []


class ExperimentGrammarError(Exception):
    pass


def get_terminal_and_noterminal_char(f=None):  # 获取终结符与非终结符
    if f is None:
        nonterminal = input("请输入非终结符(空格分开): ")
        terminal = input("请输入终结符(空格分开): ")
    else:
        nonterminal = f.readline()
        terminal = f.readline()
    Config.nonterminal_chars.extend(nonterminal.strip().split())
    Config.terminal_chars.extend(terminal.strip().split())
    Config.production_data = [[] for i in range(len(Config.nonterminal_chars))]  # 为规则数据初始化空间，否则之后直接通过Index，会报错


def get_original_grammar_text(filename=None):
    """获取源文法"""
    if filename is None:
        print("请输入文法(空行结束):")
        get_terminal_and_noterminal_char()
        start_num = 0
        while True:
            start_num += 1
            text = input("请输入第{}条文法规则: ".format(start_num))
            if not text:
                break
            yield text.strip()
    else:
        with open(filename, 'rt', encoding='utf-8') as f:
            get_terminal_and_noterminal_char(f=f)
            for line in f.readlines():
                yield line.strip()


def preprocess_grammar_text(grammar_text: str):
    """对文法中的一条规则进行预处理"""
    start_char = grammar_text[0]
    relation_chars = grammar_text[1:3]  # 这里默认使用 -> , 并未考虑 ::=
    right_chars = grammar_text[3:]
    if start_char <= 'A' or start_char >= 'Z':
        raise ExperimentGrammarError
    elif relation_chars != '->':
        raise ExperimentGrammarError
    else:
        production_list = list(right_chars.split('|'))  # 默认均为正确文法,即不出现'|'或之前没有字符的情况
        return start_char, production_list


def set_production_data(filename):
    """提取文法规则"""
    for grammar in get_original_grammar_text(filename):  # 获取输入的每一条规则
        nonterminal_char, production_list = preprocess_grammar_text(grammar)  # 对每一条规则进行预处理(提取)
        try:
            index = Config.nonterminal_chars.index(nonterminal_char)
        except ValueError:
            raise ExperimentGrammarError("文法规则左部出现不是非终结符的字符: {}".format(nonterminal_char))
        else:
            Config.production_data[index].extend(production_list)


def eliminate_left_recursion():
    """书中消除左递归算法，个人认为不对"""
    n = len(Config.nonterminal_chars)
    for i in range(n):
        for j in range(i):
            for i_production_str in Config.production_data[i]:
                if i_production_str.startswith(Config.nonterminal_chars[j]):
                    Config.production_data.remove(i_production_str)  # 删除规则
                    for j_production_str in Config.production_data[j]:
                        Config.production_data[i].append(j_production_str + i_production_str[1:])            


def eliminate_left_recursion_():
    """消除左递归算法（自实现）"""
    pass
    

def get_nullable_set():
    """可推导为空的符号的集合"""
    while True:
        old_nullable_set = deepcopy(Config.nullable_set)  # 这里用不用deepcopy无所谓
        for index in range(len(Config.nonterminal_chars)):
            for production in Config.production_data[index]:
                if production == Config.symbol_empty:
                    Config.nullable_set.add(production)
                else:
                    for char in production:  # 接受 A->B$  B->$
                        if char not in Config.nonterminal_chars and char != Config.symbol_empty:
                            break
                        if char not in old_nullable_set:
                            break
        if old_nullable_set == Config.nullable_set:
            break


def get_first_set():
    """计算FIRST集"""
    Config.first_set_list = [set() for i in range(len(Config.nonterminal_chars))]
    old_first_set_list = []
    while True:
        old_first_set_list = deepcopy(Config.first_set_list)  # Note: 必须使用deepcopy
        for index in range(len(Config.nonterminal_chars)):
            for production in Config.production_data[index]:  # 遍历每个产生式
                for first_char in production:  # 遍历每个产生式的字符
                    # 利用两个break使非空情况全部退出for,否则FIRST集将出现错误，大于实际结果
                    if first_char in Config.terminal_chars:  # 字符为终结符
                        Config.first_set_list[index] |= {first_char}  # 取并集
                        break
                    elif first_char in Config.nonterminal_chars:  # 字符为非终结符
                        n_index = Config.nonterminal_chars.index(first_char)
                        Config.first_set_list[index] |= Config.first_set_list[n_index]
                        if first_char not in Config.nullable_set:  # 如果该字符不能推出空符号串，换言之，为空即可以继续
                            break
                    else:  # 字符为空，继续循环
                        pass
        if old_first_set_list == Config.first_set_list:  # 在最小不动点处 stop loop
            break


def get_follow_set():
    """计算FOLLOW集"""
    Config.follow_set_list = [set() for i in range(len(Config.nonterminal_chars))]
    old_follow_set_list = []
    while True:
        old_follow_set_list = deepcopy(Config.follow_set_list)
        for index in range(len(Config.nonterminal_chars)):
            for production in Config.production_data[index]:
                temp_follow_set = deepcopy(Config.follow_set_list[index])  # Note: deepcopy
                for last_char in production[::-1]:  # 利用切片反转字符串
                    if last_char in Config.terminal_chars:  # 最后一个字符是终结符
                        temp_follow_set = {last_char}  # 为前一个非终结符做预留
                    elif last_char in Config.nonterminal_chars:  # 最后一个字符是非终结符
                        n_index = Config.nonterminal_chars.index(last_char)
                        Config.follow_set_list[n_index] |= temp_follow_set  # 对FOLLOW集取并集
                        if last_char not in Config.nullable_set:  # 是不能 =>*ε 的非终结符
                            temp_follow_set = Config.first_set_list[n_index]  # 直接利用该符号的FIRST集进行覆盖
                        else:  # 是能 =>*ε 的非终结符
                            temp_follow_set |= Config.first_set_list[n_index]  # 与该符号的FIRST集取并集
                    else:  # ε
                        pass
        if old_follow_set_list == Config.follow_set_list:  # 最小不动点处 stop loop
            break


def syntax_analysis():
    set_production_data(filename='test_grammar.gra')
    eliminate_left_recursion()
    get_nullable_set()
    get_first_set()
    get_follow_set()


if __name__ == '__main__':
    syntax_analysis()
    print(Config.production_data)
    print(Config.first_set_list)
    print(Config.nullable_set)
    print(Config.follow_set_list)
    