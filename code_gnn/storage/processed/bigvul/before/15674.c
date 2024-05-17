static inline int IRQ_testbit(IRQQueue *q, int n_IRQ)
{
    return test_bit(n_IRQ, q->queue);
}
