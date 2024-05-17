static uint16_t tsc2102_audio_register_read(TSC210xState *s, int reg)
{
    int l_ch, r_ch;
    uint16_t val;

    switch (reg) {
    case 0x00:	 
        return s->audio_ctrl1;

    case 0x01:
        return 0xff00;

    case 0x02:	 
        return s->volume;

    case 0x03:
        return 0x8b00;

    case 0x04:	 
        l_ch = 1;
        r_ch = 1;
        if (s->softstep && !(s->dac_power & (1 << 10))) {
            l_ch = (qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) >
                            s->volume_change + TSC_SOFTSTEP_DELAY);
            r_ch = (qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) >
                            s->volume_change + TSC_SOFTSTEP_DELAY);
        }

        return s->audio_ctrl2 | (l_ch << 3) | (r_ch << 2);

    case 0x05:	 
        return 0x2aa0 | s->dac_power |
                (((s->dac_power & (1 << 10)) &&
                  (qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) >
                   s->powerdown + TSC_POWEROFF_DELAY)) << 6);

    case 0x06:	 
        val = s->audio_ctrl3 | 0x0001;
        s->audio_ctrl3 &= 0xff3f;
        return val;

    case 0x07:	 
    case 0x08:	 
    case 0x09:	 
    case 0x0a:	 
    case 0x0b:	 
    case 0x0c:	 
    case 0x0d:	 
    case 0x0e:	 
    case 0x0f:	 
    case 0x10:	 
    case 0x11:	 
    case 0x12:	 
    case 0x13:	 
    case 0x14:	 
    case 0x15:	 
    case 0x16:	 
    case 0x17:	 
    case 0x18:	 
    case 0x19:	 
    case 0x1a:	 
        return s->filter_data[reg - 0x07];

    case 0x1b:	 
        return s->pll[0];

    case 0x1c:	 
        return s->pll[1];

    case 0x1d:	 
        return (!s->softstep) << 14;

    default:
#ifdef TSC_VERBOSE
        fprintf(stderr, "tsc2102_audio_register_read: "
                        "no such register: 0x%02x\n", reg);
#endif
        return 0xffff;
    }
}
