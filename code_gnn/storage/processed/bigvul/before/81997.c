static void _6502_anal_esil_get_addr_pattern2(RAnalOp *op, const ut8* data, char* addrbuf, int addrsize, char reg) {
	switch(data[0] & 0x1f) {  
	case 0x02:  
		op->cycles = 2;
		snprintf (addrbuf, addrsize, "0x%02x", data[1]);
		break;
	case 0x0a:  
		op->cycles = 2;
		snprintf (addrbuf, addrsize, "a");
		break;
	case 0x06:  
		op->cycles = 5;
		snprintf (addrbuf, addrsize, "0x%02x", data[1]);
		break;
	case 0x16:  
		op->cycles = 6;
		snprintf (addrbuf, addrsize, "%c,0x%02x,+", reg, data[1]);
		break;
	case 0x0e:  
		op->cycles = 6;
		snprintf (addrbuf, addrsize, "0x%04x", data[1] | data[2] << 8);
		break;
	case 0x1e:  
		op->cycles = 7;
		snprintf (addrbuf, addrsize, "%c,0x%04x,+", reg, data[1] | data[2] << 8);
		break;
	}
}
