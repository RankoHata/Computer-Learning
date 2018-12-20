import subprocess
import os
from Syntax_analysis import syntax_interface, print_

typecode_keyword    = 1  # 关键字
typecode_id         = 2  # 标识符
typecode_int        = 3  # 正整数
typecode_operator   = 4  # 操作符
typecode_delimiter  = 5  # 分隔符

dirname = 'test_grammar'  # 存储文法文件和测试字符串文件的文件夹名
grammar_filename = 'test_grammar.gra'
grammar_filepath = os.path.join(dirname, grammar_filename)  # 文法文件的路径

# out_text
# 样例
# ['<2,', 'abc>', '<4,', '+>', '<2,', 'age>', '<4,', '+>', '<3,', '80>']

def extract_data(out_text):
    data = []
    for index, str_ in enumerate(out_text.split()):
        if not index % 2:
            typecode: int = int(str_[1:-1])
        else:
            value: str = str_[:-1]
            data.append([typecode, value])
    return data


def lexical_to_syntax_interface(out_text, plain_text):
    data = extract_data(out_text)
    result = ''
    for [typecode, value] in data:
        if typecode == typecode_id or typecode == typecode_int:
            result += 'i'
        elif typecode == typecode_delimiter or typecode == typecode_operator:
            result += value
    try:
        return_value: bool = syntax_interface(grammar_filepath, result)
    except Exception:
        print_('分析过程中出错........')  # 可能文法有错啥的
    else:
        if return_value:
            print_('>>> 符号串 {} 为文法所定义的句子\n\n'.format(plain_text))
        else:
            print_('>>> 符号串 {} 不是文法所定义的句子\n\n'.format(plain_text))

    


if __name__ == '__main__':
    test_case_filename = 'test_case_1.c'
    test_case_filepath = os.path.join(dirname, test_case_filename)
    with open(test_case_filepath, 'rt', encoding='utf-8') as f:
        plain_text = f.read().strip()
    out_text = subprocess.check_output(['lexical_analysis.out', test_case_filepath]).decode('utf-8')
    lexical_to_syntax_interface(out_text, plain_text)

    test_case_filename = 'test_case_2.c'
    test_case_filepath = os.path.join(dirname, test_case_filename)
    with open(test_case_filepath, 'rt', encoding='utf-8') as f:
        plain_text = f.read().strip()
    out_text = subprocess.check_output(['lexical_analysis.out', test_case_filepath]).decode('utf-8')
    lexical_to_syntax_interface(out_text, plain_text)
