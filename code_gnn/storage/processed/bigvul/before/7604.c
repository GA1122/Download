static int cirrus_vga_read_cr(CirrusVGAState * s, unsigned reg_index)
{
    switch (reg_index) {
    case 0x00:			 
    case 0x01:			 
    case 0x02:			 
    case 0x03:			 
    case 0x04:			 
    case 0x05:			 
    case 0x06:			 
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
	return s->vga.cr[s->vga.cr_index];
    case 0x24:			 
        return (s->vga.ar_flip_flop << 7);
    case 0x19:			 
    case 0x1a:			 
    case 0x1b:			 
    case 0x1c:			 
    case 0x1d:			 
    case 0x22:			 
    case 0x25:			 
    case 0x27:			 
	return s->vga.cr[s->vga.cr_index];
    case 0x26:			 
	return s->vga.ar_index & 0x3f;
	break;
    default:
#ifdef DEBUG_CIRRUS
	printf("cirrus: inport cr_index %02x\n", reg_index);
#endif
	return 0xff;
    }
}
