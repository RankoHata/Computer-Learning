mutex_lock:  ; mutex_lock与mutex_unlock实现
    TSL REGISTER, MUTEX  ; 将互斥信号复制到寄存器，并将互斥信号置1
    CMP REGISTER, #0
    JZE ok  ; 等于0，跳转
    CALL thread_yield  ; 互斥信号量忙，调度另一个线程
    JMP mutex_lock  ; loop
ok:
    RET


mutex_unlock:
    MOVE MUTEX, #0
    RET