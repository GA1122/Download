static void tsc2102_audio_register_write(
                TSC210xState *s, int reg, uint16_t value)
{
    switch (reg) {
    case 0x00:	 
        s->audio_ctrl1 = value & 0x0f3f;
#ifdef TSC_VERBOSE
        if ((value & ~0x0f3f) || ((value & 7) != ((value >> 3) & 7)))
            fprintf(stderr, "tsc2102_audio_register_write: "
                            "wrong value written into Audio 1\n");
#endif
        tsc2102_audio_rate_update(s);
        tsc2102_audio_output_update(s);
        return;

    case 0x01:
#ifdef TSC_VERBOSE
        if (value != 0xff00)
            fprintf(stderr, "tsc2102_audio_register_write: "
                            "wrong value written into reg 0x01\n");
#endif
        return;

    case 0x02:	 
        s->volume = value;
        s->volume_change = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        return;

    case 0x03:
#ifdef TSC_VERBOSE
        if (value != 0x8b00)
            fprintf(stderr, "tsc2102_audio_register_write: "
                            "wrong value written into reg 0x03\n");
#endif
        return;

    case 0x04:	 
        s->audio_ctrl2 = value & 0xf7f2;
#ifdef TSC_VERBOSE
        if (value & ~0xf7fd)
            fprintf(stderr, "tsc2102_audio_register_write: "
                            "wrong value written into Audio 2\n");
#endif
        return;

    case 0x05:	 
        if ((value & ~s->dac_power) & (1 << 10))
            s->powerdown = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);

        s->dac_power = value & 0x9543;
#ifdef TSC_VERBOSE
        if ((value & ~0x9543) != 0x2aa0)
            fprintf(stderr, "tsc2102_audio_register_write: "
                            "wrong value written into Power\n");
#endif
        tsc2102_audio_rate_update(s);
        tsc2102_audio_output_update(s);
        return;

    case 0x06:	 
        s->audio_ctrl3 &= 0x00c0;
        s->audio_ctrl3 |= value & 0xf800;
#ifdef TSC_VERBOSE
        if (value & ~0xf8c7)
            fprintf(stderr, "tsc2102_audio_register_write: "
                            "wrong value written into Audio 3\n");
#endif
        tsc2102_audio_output_update(s);
        return;

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
        s->filter_data[reg - 0x07] = value;
        return;

    case 0x1b:	 
        s->pll[0] = value & 0xfffc;
#ifdef TSC_VERBOSE
        if (value & ~0xfffc)
            fprintf(stderr, "tsc2102_audio_register_write: "
                            "wrong value written into PLL 1\n");
#endif
        return;

    case 0x1c:	 
        s->pll[1] = value & 0xfffc;
#ifdef TSC_VERBOSE
        if (value & ~0xfffc)
            fprintf(stderr, "tsc2102_audio_register_write: "
                            "wrong value written into PLL 2\n");
#endif
        return;

    case 0x1d:	 
        s->softstep = !(value & 0x4000);
#ifdef TSC_VERBOSE
        if (value & ~0x4000)
            fprintf(stderr, "tsc2102_audio_register_write: "
                            "wrong value written into Audio 4\n");
#endif
        return;

    default:
#ifdef TSC_VERBOSE
        fprintf(stderr, "tsc2102_audio_register_write: "
                        "no such register: 0x%02x\n", reg);
#endif
    }
}
