static void cirrus_vga_write_cr(CirrusVGAState * s, int reg_value)
{
    switch (s->vga.cr_index) {
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
	 
	if ((s->vga.cr[0x11] & 0x80) && s->vga.cr_index <= 7) {
	     
	    if (s->vga.cr_index == 7)
		s->vga.cr[7] = (s->vga.cr[7] & ~0x10) | (reg_value & 0x10);
	    return;
	}
	s->vga.cr[s->vga.cr_index] = reg_value;
	switch(s->vga.cr_index) {
	case 0x00:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
	case 0x11:
	case 0x17:
	    s->vga.update_retrace_info(&s->vga);
	    break;
	}
        break;
    case 0x19:			 
    case 0x1a:			 
    case 0x1b:			 
    case 0x1c:			 
    case 0x1d:			 
	s->vga.cr[s->vga.cr_index] = reg_value;
#ifdef DEBUG_CIRRUS
	printf("cirrus: handled outport cr_index %02x, cr_value %02x\n",
	       s->vga.cr_index, reg_value);
#endif
	break;
    case 0x22:			 
    case 0x24:			 
    case 0x26:			 
    case 0x27:			 
	break;
    case 0x25:			 
    default:
#ifdef DEBUG_CIRRUS
	printf("cirrus: outport cr_index %02x, cr_value %02x\n",
               s->vga.cr_index, reg_value);
#endif
	break;
    }
}
