static void pl022_reset(PL022State *s)
{
    s->rx_fifo_len = 0;
    s->tx_fifo_len = 0;
    s->im = 0;
    s->is = PL022_INT_TX;
    s->sr = PL022_SR_TFE | PL022_SR_TNF;
}
