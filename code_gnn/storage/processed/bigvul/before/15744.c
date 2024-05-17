static void pl022_xfer(PL022State *s)
{
    int i;
    int o;
    int val;

    if ((s->cr1 & PL022_CR1_SSE) == 0) {
        pl022_update(s);
        DPRINTF("Disabled\n");
        return;
    }

    DPRINTF("Maybe xfer %d/%d\n", s->tx_fifo_len, s->rx_fifo_len);
    i = (s->tx_fifo_head - s->tx_fifo_len) & 7;
    o = s->rx_fifo_head;
     
    while (s->tx_fifo_len && s->rx_fifo_len < 8) {
        DPRINTF("xfer\n");
        val = s->tx_fifo[i];
        if (s->cr1 & PL022_CR1_LBM) {
             
        } else {
            val = ssi_transfer(s->ssi, val);
        }
        s->rx_fifo[o] = val & s->bitmask;
        i = (i + 1) & 7;
        o = (o + 1) & 7;
        s->tx_fifo_len--;
        s->rx_fifo_len++;
    }
    s->rx_fifo_head = o;
    pl022_update(s);
}
