static void i6300esb_mem_writel(void *vp, hwaddr addr, uint32_t val)
{
    I6300State *d = vp;

    i6300esb_debug ("addr = %x, val = %x\n", (int) addr, val);

    if (addr == 0xc && val == 0x80)
        d->unlock_state = 1;
    else if (addr == 0xc && val == 0x86 && d->unlock_state == 1)
        d->unlock_state = 2;
    else {
        if (d->unlock_state == 2) {
            if (addr == 0)
                d->timer1_preload = val & 0xfffff;
            else if (addr == 4)
                d->timer2_preload = val & 0xfffff;

            d->unlock_state = 0;
        }
    }
}
