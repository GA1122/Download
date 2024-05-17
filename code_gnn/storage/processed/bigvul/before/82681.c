static ut64 disarm_8bit_offset (ut64 pc, ut32 offs) {
         
        ut64 off = offs;
	 
	if ((off & 0x80) == 0x80)
	{
		off |= ~0xFF;
	}
	return (off<<1) + pc + 4;
}
