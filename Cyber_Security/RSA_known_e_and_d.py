str_item = 'HAPPY WEEK END'  # 明文
p = 47
q = 73
d = 167  # 私钥
n = p * q
e = 119  # 公钥

x = {chr(i):i-96 for i in range(97, 97+26)}  # 英文字母映射
x[' '] = 0


def encode_(str_):  # 数据编码 00: 空格; 01: a; 02: b; ..... 26: z;
    '''
    数据明文:  HAPPY WEEK END
    数据编码:  0801 1616 2500 2305 0511 0005 1404
    '''
    encode_str = ''
    signal = 0
    for i in str_:
        if signal % 2 == 0 and signal != 0:
            encode_str += ' '
        encode_str += str(x[i.lower()]).zfill(2)
        signal += 1
    return encode_str


def encrypt_(encode_str, key=e):  # 加密
    result_str = ''
    for i in encode_str.split(' '):
        num = int(i)
        result = num**key % n
        result_str += str(result).zfill(4)
        result_str += ' '
    return result_str.rstrip()


def decrypt_(encrypt_str, key=d):  # 解密
    return encrypt_(encrypt_str, key=d)


encode_str = encode_(str_item)
encrypt_str = encrypt_(encode_str)
decrypt_str = decrypt_(encrypt_str)

assert encode_str == decrypt_str  # 断言RSA算法正确性


print('数据明文: ', str_item)
print('数据编码: ', encode_str)
print('加密密文: ', encrypt_str)
print('解密密文: ', decrypt_str)
