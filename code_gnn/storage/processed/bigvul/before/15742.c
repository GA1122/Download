static void pl022_update(PL022State *s)
{
    s->sr = 0;
    if (s->tx_fifo_len == 0)
        s->sr |= PL022_SR_TFE;
    if (s->tx_fifo_len != 8)
        s->sr |= PL022_SR_TNF;
    if (s->rx_fifo_len != 0)
        s->sr |= PL022_SR_RNE;
    if (s->rx_fifo_len == 8)
        s->sr |= PL022_SR_RFF;
    if (s->tx_fifo_len)
        s->sr |= PL022_SR_BSY;
    s->is = 0;
    if (s->rx_fifo_len >= 4)
        s->is |= PL022_INT_RX;
    if (s->tx_fifo_len <= 4)
        s->is |= PL022_INT_TX;

    qemu_set_irq(s->irq, (s->is & s->im) != 0);
}
