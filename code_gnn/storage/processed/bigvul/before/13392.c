static void i6300esb_mem_writew(void *vp, hwaddr addr, uint32_t val)
{
    I6300State *d = vp;

    i6300esb_debug("addr = %x, val = %x\n", (int) addr, val);

    if (addr == 0xc && val == 0x80)
        d->unlock_state = 1;
    else if (addr == 0xc && val == 0x86 && d->unlock_state == 1)
        d->unlock_state = 2;
    else {
        if (d->unlock_state == 2) {
            if (addr == 0xc) {
                if ((val & 0x100) != 0)
                     
                    i6300esb_restart_timer(d, 1);

                 
                if ((val & 0x200) != 0 || (val & 0x1000) != 0) {
                    d->previous_reboot_flag = 0;
                }
            }

            d->unlock_state = 0;
        }
    }
}
