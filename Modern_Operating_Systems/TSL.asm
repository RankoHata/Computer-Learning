enter_region:  ; 用TSL指令进入和离开临界区
    TSL REGISTER, LOCK  ; 复制锁到寄存器并将锁置为1
    CMP REGISTER, #0  ; 锁是0吗
    JNE enter_region  ; 若不是0,说明锁已被设置,loop
    RET


leave_region:
    MOVE LOCK, #0  ; 将锁置为0
    RET
