enter_region:  ; 用XCHG指令进入和离开临界区
    MOVE REGISTER, #1
    XCHG REGISTER, LOCK  ; 交换寄存器与锁的内容
    CMP REGISTER, #0
    JNE enter_region
    RET


leave_region:
    MOVE LOCK, #0
    RET
