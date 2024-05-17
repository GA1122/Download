static uint64_t pl022_read(void *opaque, hwaddr offset,
                           unsigned size)
{
    PL022State *s = (PL022State *)opaque;
    int val;

    if (offset >= 0xfe0 && offset < 0x1000) {
        return pl022_id[(offset - 0xfe0) >> 2];
    }
    switch (offset) {
    case 0x00:  
      return s->cr0;
    case 0x04:  
      return s->cr1;
    case 0x08:  
        if (s->rx_fifo_len) {
            val = s->rx_fifo[(s->rx_fifo_head - s->rx_fifo_len) & 7];
            DPRINTF("RX %02x\n", val);
            s->rx_fifo_len--;
            pl022_xfer(s);
        } else {
            val = 0;
        }
        return val;
    case 0x0c:  
        return s->sr;
    case 0x10:  
        return s->cpsr;
    case 0x14:  
        return s->im;
    case 0x18:  
        return s->is;
    case 0x1c:  
        return s->im & s->is;
    case 0x20:  
         
        return 0;
    default:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "pl022_read: Bad offset %x\n", (int)offset);
        return 0;
    }
}
