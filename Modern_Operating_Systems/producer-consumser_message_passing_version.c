/* 用N条消息实现的生产者-消费者问题 */

#define N 100  // 缓冲区槽数

void producer(void){
    int item;
    message m;  // 消息缓冲区

    while(TRUE){
        item = produce_item();
        receive(consumer, &m);  // 等待消费者发送空缓冲区
        build_message(&m, item);
        send(consumer, &m);
    }
}

void consumer(void){
    int item, i;
    message m;

    for(i = 0; i < N; i++) send(producer, &m);  // 发送N个空缓冲区
    while(TRUE){
        receive(producer, &m);
        item = extract_item(&m);  // 提取数据
        send(producer, &m);  // 将空缓冲区发送回生产者
        consume_item(item);  // 处理数据项
    }
}

/* 
消费者首先将N条空消息发送给生产者。当生产者向消费者传递一个数据项时，
它取走一条空消息并送回一条填充了内容的消息。通过这种方式，
系统中总的消息数保持不变，所以消息都可以存放在事先确定数量的内存中。
*/