"""实现LL(1)语法分析

实验文法约定:
    1. 输入文法中无多余空格
    2. 使用字符$(代表ε)
    3. 非终结符仅支持单个字符,且均为大写字母
    4. 终结符均为ASCII字符
    5. 定义关系仅能识别->(暂不支持::=)
    6. 默认输入的第一个非终结符为文法开始符号
    7. 本程序能够识别出一些输入文法的错误情况,但是仍旧有很多情况,
       是默认输入文法正确的,所以不要尝试依赖程序的报错功能,这并不稳定

文法文件约定:
    1. 放入当前目录下test_grammar文件夹
    2. 文法文件后缀为.gra
    3. 文件名为test_grammar{n}.gra (n为数字)
    4. 第一行为所有的非终结符,单空格分隔
    5. 第二行为所有的终结符,单空格分隔
    6. 从第三行开始,每行一条文法规则

测试字符串文件:
    1. 文件名为test_token{n} (n为数字)
    2. 每行一个测试字符串

实验警告:
    1. 务必按照要求构造文法文件与测试文件,否则会引起看似完全不合理的情况或不合理的结果
"""

import os  # 路径相关操作
from copy import deepcopy  # 深拷贝


def print_(*args, **kwargs):
    """因为在编码时的愚蠢设计,导致将输出改为文件时,过于麻烦,故封装print函数,简化操作"""
    with open('analysis_result.out', 'at', encoding='utf-8') as f:
        print(*args, **kwargs, file=f)  # 输出至文件


class Config(object):
    """供其他函数使用的变量类,愚蠢的设计"""
    start_char = None           # 文法开始符号
    nonterminal_chars = []      # 非终结符 VN
    terminal_chars = []         # 终结符 VT
    production_data = []        # 产生式
    first_set_list = []         # FIRST集
    follow_set_list = []        # FOLLOW集
    nullable_set = set()        # NULLABLE集 (可以推导出ε的非终结符，而非必然推导出ε的终结符)
    first_s_set_list = []       # FIRST_S集 (每个产生式在文法分析表中对应的非终结符,再根据下标位置,可得出该产生式在文法分析表中的位置)
    symbol_empty = '$'          # 空符号 ε

    SUCCESS = 0     # 字符串是该文法句子
    ERROR_1 = 1     # 当余留输入串不再有字符时,对应分析栈内的非终结符也不能推导出ε
    ERROR_2 = 2     # 分析栈 与 余留输入串 直接对应的终结符不相同
    ERROR_3 = 3     # 分析栈已空,余留字符串不为空
    ERROR_4 = 4     # 查文法分析表,无对应项

    ERROR_MESSAGE = {  # 错误信息
        SUCCESS: '字符串是该文法句子',
        ERROR_1: '当余留输入串不再有字符时,对应分析栈内的非终结符也不能推导出ε',
        ERROR_2: '分析栈 与 余留输入串 直接对应的终结符不相同',
        ERROR_3: '分析栈已空,余留字符串不为空',
        ERROR_4: '查文法分析表,无对应项',
    }

class GrammarError(Exception):
    """输入的文法错误"""
    pass

class ProgramLimitError(Exception):
    """超出当前编写的代码限制而引发的错误(例如:输入文法,超出该语法分析的文法范围)"""
    pass


def clear():
    """因为愚蠢的设计了Config这个类,导致需要写函数对静态类Config进行数值初始化"""
    Config.start_char = None
    Config.nonterminal_chars = []
    Config.terminal_chars = []
    Config.production_data = []
    Config.first_set_list = []
    Config.follow_set_list = []
    Config.nullable_set = set()


def __get_terminal_and_noterminal_char(f):
    """通过分析文本获取终结符与非终结符
    Args:
        f:  gra文法文件的文件描述符
    """
    nonterminal = f.readline()  # gra文法文件第一行
    terminal = f.readline()  # gra文法文件第二行
    Config.nonterminal_chars.extend(nonterminal.strip().split())
    Config.terminal_chars.extend(terminal.strip().split())
    Config.production_data = [[] for i in range(len(Config.nonterminal_chars))]  # 为规则数据初始化空间，否则之后直接通过Index，会报错


def __get_original_grammar_text(filepath: str):
    """获取原文法的生成器,每次返回一行规则"""
    with open(filepath, 'rt', encoding='utf-8') as f:
        __get_terminal_and_noterminal_char(f=f)
        for line in f.readlines():
            yield line.strip()


def __preprocess_grammar_text(grammar_text: str):
    """对文法中的一条规则(一行)进行预处理
    Example:
        A->B|a
    """
    left_char = grammar_text[0]
    relation_chars = grammar_text[1:3]  # 这里默认使用 -> , 并未考虑 ::=
    right_chars = grammar_text[3:]
    if left_char < 'A' or left_char > 'Z':
        errmsg = '终结符: {} 超出了本词法分析程序规定的范围'.format(left_char)
        print_('Error: ' + errmsg)
        raise ProgramLimitError(errmsg)
    elif relation_chars != '->':
        errmsg = '不可识别的关系符号: {}'.format(relation_chars)
        print_('Error: ' + errmsg)
        raise GrammarError(errmsg)
    else:
        production_list = list(right_chars.split('|'))  # 默认输入文法均为正确文法,即不出现'|'或之前没有字符的情况
        return left_char, production_list


def set_production_data(filepath):
    """提取文法产生式数据"""
    for grammar in __get_original_grammar_text(filepath):  # 获取输入的每一条规则
        nonterminal_char, production_list = __preprocess_grammar_text(grammar)  # 对每一条规则进行预处理(提取)
        try:
            index = Config.nonterminal_chars.index(nonterminal_char)
        except ValueError:
            errmsg = "文法规则左部出现不是非终结符的字符: {}".format(nonterminal_char)
            print_('Error: ' + errmsg)
            raise GrammarError(errmsg)
        else:
            Config.production_data[index].extend(production_list)  # 将产生式存储在对应非终结符处
    Config.start_char = Config.nonterminal_chars[0]  # 设置文法开始符号


def eliminate_indirect_left_recursion():
    """消除间接左递归算法
    1. 将文法G的所有非终结符整理成某种顺序U1, U2, ..., Un
    2. FOR i:= 1 TO n DO:
        BEGIN
            FOR j:= 1 TO i-1 DO
            BEGIN
            IF Ui ::= Uj_y
                Ui ::= x1_y | x2_y | ... | xk_y (Uj ::= x1 | x2 | ... | xk 是Uj的所有规则)
            END
        END
    3. 去除多于规则(并没有做) 
    """
    n = len(Config.nonterminal_chars)
    production_data = deepcopy(Config.production_data)
    for i in range(n):
        for j in range(i):
            for i_production_str in production_data[i]:
                if i_production_str.startswith(Config.nonterminal_chars[j]):
                    Config.production_data[i].remove(i_production_str)  # 删除规则
                    for j_production_str in Config.production_data[j]:
                        Config.production_data[i].append(j_production_str + i_production_str[1:])


def eliminate_direct_left_recursion():
    """消除直接左递归算法
    形如 A::=Aα|β
    其中α非空,β不以A开头,则可等价修改为
    A::=βA'         (new_production_1)
    A'::=αA'|ε      (new_production_2)
    
    Returns:
        replacement_map     dict    存储消除直接左递归时产生的新终结符与产生其的原终结符的映射 (仅仅为了显示提示信息)
    """
    production_data = deepcopy(Config.production_data)
    nonterminal_chars = deepcopy(Config.nonterminal_chars)
    replacement_map = {}  # 存储消除直接左递归时产生的新终结符与产生其的原终结符的映射 (仅仅为了显示提示信息)

    for i, production_list in enumerate(production_data):
        recursion_flag = False  # 某个非终结符的所有产生式中是否存在直接左递归
        alpha, beta = set(), set()  # α, β
        nonterminal_char = nonterminal_chars[i]
        for production in production_list:
            if production.startswith(nonterminal_char):
                recursion_flag = True
        if recursion_flag:
            for production in production_list:
                if production[0] == nonterminal_char:
                    if production[1:] == '':
                        errmsg = '消除直接左递归错误: 存在错误的产生式: {}'.format(nonterminal_char + '->' + production)
                        print_('Error: ' + errmsg)
                        raise GrammarError(errmsg)
                    else:
                        alpha.add(production[1:])
                else:
                    beta.add(production)
            if beta == set():
                errmsg = '消除直接左递归错误: 存在无法消除左递归的产生式'
                print_('Error: ' + errmsg)
                raise GrammarError(errmsg)
            
            # 为新非终结符分配字符
            new_nonterminal_char = None
            for num in range(26):
                available_char = chr(ord('A') + num)
                if available_char not in Config.nonterminal_chars:
                    new_nonterminal_char = available_char
                    break
            if new_nonterminal_char is None:
                errmsg = '所有大写英文字符已被用完,没有字符可分配给新的终结符'
                print_('Error: ' + errmsg)
                raise ProgramLimitError(errmsg)
            replacement_map[new_nonterminal_char] = nonterminal_char

            Config.production_data[i] = [item + new_nonterminal_char for item in beta]  # 如果存在A::=ε 可能会出问题
            Config.nonterminal_chars.append(new_nonterminal_char)
            temp = [item + new_nonterminal_char for item in alpha]
            temp.append(Config.symbol_empty)
            Config.production_data.append(temp)
    return replacement_map


def get_nullable_set():
    """计算NULLABLE集(可推导为空的符号的集合)"""
    while True:
        flag = True
        old_nullable_set = deepcopy(Config.nullable_set)  # 单维可变数据用不用deepcopy无所谓
        for index in range(len(Config.nonterminal_chars)):
            for production in Config.production_data[index]:
                if production == Config.symbol_empty:
                    Config.nullable_set.add(Config.nonterminal_chars[index])
                else:
                    for char in production:  # 接受 A->B$  B->$
                        if char not in Config.nonterminal_chars and char != Config.symbol_empty:
                            flag = False
                            break
                        if char not in old_nullable_set:
                            flag = False
                            break
                    if flag:
                        Config.nullable_set.add(Config.nonterminal_chars[index])
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


def get_first_s_set():
    """计算FIRST_S集"""
    for item in Config.production_data:  # 开辟数据空间
        Config.first_s_set_list.append([])
        for production in item:
            Config.first_s_set_list[-1].append(set())

    for i, nonterminal in enumerate(Config.nonterminal_chars):
        for j, production in enumerate(Config.production_data[i]):
            __calculte_first_s_set(production, i, j)


def __calculte_first_s_set(production, i, j):
    """计算一个产生式的FIRST_S集"""
    for first_char in production:
        if first_char in Config.terminal_chars:
            Config.first_s_set_list[i][j] |= {first_char}
            return
        elif first_char in Config.nonterminal_chars:
            n_index = Config.nonterminal_chars.index(first_char)
            Config.first_s_set_list[i][j] |= Config.first_set_list[n_index]
            if first_char not in Config.nullable_set:
                return
    Config.first_s_set_list[i][j] |= Config.follow_set_list[i]


def check_is_LL1():
    """检测该文法是否是LL(1)文法"""
    for item in Config.first_s_set_list:
        # 若同一终结符的任意两个FIRST_S集存在交集,即文法分析表中单项规则个数>1,该文法不是LL(1)文法
        for i, temp_set_i in enumerate(item):
            for temp_set_j in item[i + 1:]:
                if temp_set_i & temp_set_j:
                    errmsg = '该文法不是LL(1)文法'
                    print_('Error: ' + errmsg)
                    raise GrammarError(errmsg)


def parse(text: str):
    """分析输入符号串的功能函数
    Args:
        text:   待分析的符号串
    Returns:
        分析的状态: 成功 or 错误代码
    """
    print_('>>> 分析符号串: {}'.format(text))
    # formatter = '步骤: {:<3}  分析栈: {:<10}  余留输入串: {:>10}  所用产生式: {:20}'
    formatter = ' {:<4}  {:<10}  {:>10}   {:20}'
    cn_formatter = ' {:<2}  {:<5}  {:>7}   {:10}'
    print_(cn_formatter.format('步骤', '分析栈', '余留输入串', '所用产生式'))
    index = 1
    rule = ''
    stack = [Config.nonterminal_chars[0]]  # 模拟堆栈,文法开始符号入栈
    remain_token = text
    # print_(formatter.format('步骤', '分析栈', '余留输入串', '所用产生式'))
    while len(stack) != 0:
        while remain_token[:1] == Config.symbol_empty:  # 消除余留输入串中开头的ε
            remain_token = remain_token[1:]

        if stack[-1] in Config.nonterminal_chars:
            i = Config.nonterminal_chars.index(stack[-1])
            if remain_token[:1] == '':  # 对程序最后空符号部分进行特殊处理,因为程序内没有'#',所以这里要特判
                if Config.symbol_empty in Config.production_data[i]:
                    rule += stack[-1] + '->' + Config.symbol_empty
                else:
                    return Config.ERROR_1  # 当余留输入串不再有字符时,对应分析栈内的非终结符也不能推导出ε
            else:
                for j, terminal_set in enumerate(Config.first_s_set_list[i]):
                    if remain_token[:1] in terminal_set:  # 使用:1,可以避免remain_token为空时,下标越界的错误
                        rule += stack[-1] + '->' + Config.production_data[i][j]
                        break
                if rule == '':
                    return Config.ERROR_4  # 查文法分析表,无对应项
            stack_str = ''.join(stack)
            print_(formatter.format(index, stack_str, remain_token, rule))
            stack.pop()
            temp_production = [str_ for str_ in rule.split('->')[-1][::-1] if str_ != Config.symbol_empty]  # 分析时忽略空符号
            stack.extend(temp_production)
        elif stack[-1] in Config.terminal_chars:
            if stack[-1] == remain_token[:1]:
                stack_str = ''.join(stack)
                print_(formatter.format(index, stack_str, remain_token, rule))
                stack.pop()
                remain_token = remain_token[1:]
            else:
                return Config.ERROR_2  # 分析栈 与 余留输入串 直接对应的终结符不相同
        rule = ''
        index += 1
    for char in remain_token:
        if char != Config.symbol_empty:
            return Config.ERROR_3  # 分析栈已空,余留字符串不为空
    print_(formatter.format(index, '', '', 'success'))
    return Config.SUCCESS


def output_vn_and_production():
    """输出非终结符和产生式"""
    formatter = '非终结符: {:5} 产生式: {:20}'
    for item in zip(Config.nonterminal_chars, Config.production_data):
        print_(formatter.format(*[str(i) for i in item]))


def output_all_info():
    """输出非终结符,产生式,FIRST集,FOLLOW集,FIRST_S集"""
    formatter = '非终结符: {:5} 产生式: {:20} FIRST: {:20} FOLLOW: {:20} FIRST_S: {:20}'
    for item in zip(Config.nonterminal_chars, Config.production_data, Config.first_set_list, Config.follow_set_list, Config.first_s_set_list):
        print_(formatter.format(*[str(i) for i in item]))


def syntax_analysis(text, filepath):
    """LL(1)语法分析的接口
    Args:
        text:       待分析字符串
        filepath:   文法文件路径
    """
    clear()  # 重置Config的变量值
    print_('使用文法:')
    with open(filepath, 'rt', encoding='utf-8') as f:
        print_(f.read())  # 输出输入的原始文法
    print_('-----------------待分析输入串: {}------------------'.format(text))
    set_production_data(filepath)  # 从文法文本数据中提取文法规则
    print_('>>> 提取的原始文法:')
    output_vn_and_production()
    eliminate_indirect_left_recursion()  # 消除间接左递归
    print_('>>> 消除间接左递归:')
    output_vn_and_production()
    replacement_map: dict = eliminate_direct_left_recursion()  # 消除直接左递归
    print_('>>> 消除直接左递归:')
    for new, old in replacement_map.items():
        print_('使用新非终结符 {} 代替 {}\''.format(new, old))  # 输出替代信息
    output_vn_and_production()

    get_nullable_set()  # 计算NULLABLE集
    print_('>>> NULLABLE集: {}'.format(str(Config.nullable_set)))
    get_first_set()     # 计算FIRST集
    get_follow_set()    # 计算FOLLOW集
    get_first_s_set()   # 计算FIRST_S集
    check_is_LL1()      # 检测是否是LL(1)文法
    print_('>>> 计算FIRST集,FOLLOW集,FIRST_S集:')
    output_all_info()

    status_code = parse(text)  # 分析字符串,获取状态码
    if status_code == Config.SUCCESS:
        print_('>>> 分析完成,字符串: {} 是该文法的句子'.format(text))
        return True
    else:
        print_('>>> 分析完成,字符串: {} 不是该文法的句子\n>>> 错误代码: {}\n>>> 错误信息: {}'.format(text, status_code, Config.ERROR_MESSAGE[status_code]))
        return False


def syntax_interface(grammar_filepath, text):
    return syntax_analysis(text, grammar_filepath)


if __name__ == '__main__':
    dirname = 'test_grammar'  # 存储文法文件和测试字符串文件的文件夹名
    test_filename = 'test_token'
    grammar_filename = 'test_grammar1.gra'
    with open(os.path.join(dirname, test_filename), 'rt', encoding='utf-8') as f:  # 测试文件一行一个测试字符串
        for line in f.readlines():
            syntax_analysis(line.strip(), filepath=os.path.join(dirname, grammar_filename))  # 注意使用strip,否则\n会干扰程序执行
