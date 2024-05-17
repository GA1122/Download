Luv24fromLuv48(LogLuvState* sp, uint8* op, tmsize_t n)
{
	uint32* luv = (uint32*) sp->tbuf;  
	int16* luv3 = (int16*) op;

	while (n-- > 0) {
		int Le, Ce;

		if (luv3[0] <= 0)
			Le = 0;
		else if (luv3[0] >= (1<<12)+3314)
			Le = (1<<10) - 1;
		else if (sp->encode_meth == SGILOGENCODE_NODITHER)
			Le = (luv3[0]-3314) >> 2;
		else
			Le = itrunc(.25*(luv3[0]-3314.), sp->encode_meth);

		Ce = uv_encode((luv3[1]+.5)/(1<<15), (luv3[2]+.5)/(1<<15),
					sp->encode_meth);
		if (Ce < 0)	 
			Ce = uv_encode(U_NEU, V_NEU, SGILOGENCODE_NODITHER);
		*luv++ = (uint32)Le << 14 | Ce;
		luv3 += 3;
	}
}