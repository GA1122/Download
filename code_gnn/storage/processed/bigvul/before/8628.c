static inline void eth_pulse_irq(struct xlx_ethlite *s)
{
     
    if (s->regs[R_TX_GIE0] & GIE_GIE) {
        qemu_irq_pulse(s->irq);
    }
}
