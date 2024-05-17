static inline void IRQ_setbit(IRQQueue *q, int n_IRQ)
{
    set_bit(n_IRQ, q->queue);
}
