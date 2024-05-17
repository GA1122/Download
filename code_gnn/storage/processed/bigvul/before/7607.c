static int cirrus_vga_read_sr(CirrusVGAState * s)
{
    switch (s->vga.sr_index) {
    case 0x00:			 
    case 0x01:			 
    case 0x02:			 
    case 0x03:			 
    case 0x04:			 
	return s->vga.sr[s->vga.sr_index];
    case 0x06:			 
	return s->vga.sr[s->vga.sr_index];
    case 0x10:
    case 0x30:
    case 0x50:
    case 0x70:			 
    case 0x90:
    case 0xb0:
    case 0xd0:
    case 0xf0:			 
	return s->vga.sr[0x10];
    case 0x11:
    case 0x31:
    case 0x51:
    case 0x71:			 
    case 0x91:
    case 0xb1:
    case 0xd1:
    case 0xf1:			 
	return s->vga.sr[0x11];
    case 0x05:			 
    case 0x07:			 
    case 0x08:			 
    case 0x09:			 
    case 0x0a:			 
    case 0x0b:			 
    case 0x0c:			 
    case 0x0d:			 
    case 0x0e:			 
    case 0x0f:			 
    case 0x12:			 
    case 0x13:			 
    case 0x14:			 
    case 0x15:			 
    case 0x16:			 
    case 0x17:			 
    case 0x18:			 
    case 0x19:			 
    case 0x1a:			 
    case 0x1b:			 
    case 0x1c:			 
    case 0x1d:			 
    case 0x1e:			 
    case 0x1f:			 
#ifdef DEBUG_CIRRUS
	printf("cirrus: handled inport sr_index %02x\n", s->vga.sr_index);
#endif
	return s->vga.sr[s->vga.sr_index];
    default:
#ifdef DEBUG_CIRRUS
	printf("cirrus: inport sr_index %02x\n", s->vga.sr_index);
#endif
	return 0xff;
	break;
    }
}
