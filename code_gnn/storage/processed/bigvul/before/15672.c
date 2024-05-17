static inline void IRQ_resetbit(IRQQueue *q, int n_IRQ)
{
    clear_bit(n_IRQ, q->queue);
}
