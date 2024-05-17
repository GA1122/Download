static void cirrus_vga_write_sr(CirrusVGAState * s, uint32_t val)
{
    switch (s->vga.sr_index) {
    case 0x00:			 
    case 0x01:			 
    case 0x02:			 
    case 0x03:			 
    case 0x04:			 
	s->vga.sr[s->vga.sr_index] = val & sr_mask[s->vga.sr_index];
	if (s->vga.sr_index == 1)
            s->vga.update_retrace_info(&s->vga);
        break;
    case 0x06:			 
	val &= 0x17;
	if (val == 0x12) {
	    s->vga.sr[s->vga.sr_index] = 0x12;
	} else {
	    s->vga.sr[s->vga.sr_index] = 0x0f;
	}
	break;
    case 0x10:
    case 0x30:
    case 0x50:
    case 0x70:			 
    case 0x90:
    case 0xb0:
    case 0xd0:
    case 0xf0:			 
	s->vga.sr[0x10] = val;
	s->hw_cursor_x = (val << 3) | (s->vga.sr_index >> 5);
	break;
    case 0x11:
    case 0x31:
    case 0x51:
    case 0x71:			 
    case 0x91:
    case 0xb1:
    case 0xd1:
    case 0xf1:			 
	s->vga.sr[0x11] = val;
	s->hw_cursor_y = (val << 3) | (s->vga.sr_index >> 5);
	break;
    case 0x07:			 
    cirrus_update_memory_access(s);
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
    case 0x18:			 
    case 0x19:			 
    case 0x1a:			 
    case 0x1b:			 
    case 0x1c:			 
    case 0x1d:			 
    case 0x1e:			 
    case 0x1f:			 
	s->vga.sr[s->vga.sr_index] = val;
#ifdef DEBUG_CIRRUS
	printf("cirrus: handled outport sr_index %02x, sr_value %02x\n",
	       s->vga.sr_index, val);
#endif
	break;
    case 0x17:			 
	s->vga.sr[s->vga.sr_index] = (s->vga.sr[s->vga.sr_index] & 0x38)
                                   | (val & 0xc7);
        cirrus_update_memory_access(s);
        break;
    default:
#ifdef DEBUG_CIRRUS
	printf("cirrus: outport sr_index %02x, sr_value %02x\n",
               s->vga.sr_index, val);
#endif
	break;
    }
}
