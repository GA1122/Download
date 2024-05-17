static void tsc2102_data_register_write(
                TSC210xState *s, int reg, uint16_t value)
{
    switch (reg) {
    case 0x00:	 
    case 0x01:	 
    case 0x02:	 
    case 0x03:	 
    case 0x05:	 
    case 0x06:	 
    case 0x07:	 
    case 0x08:	 
    case 0x09:	 
    case 0x0a:	 
        return;

    default:
#ifdef TSC_VERBOSE
        fprintf(stderr, "tsc2102_data_register_write: "
                        "no such register: 0x%02x\n", reg);
#endif
    }
}
