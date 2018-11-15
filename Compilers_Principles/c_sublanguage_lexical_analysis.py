class Buf(object):
    def __init__(self):
        self.__buf1 = []
        self.__buf2 = []
        self.__current_buf = 0
        self.pos = 0
        self.max_buf_size = 120
    
    def get_char(self):
        num = self.pos 