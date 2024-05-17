do_des(uint32 l_in, uint32 r_in, uint32 *l_out, uint32 *r_out, int count)
{
	 
	uint32		l,
				r,
			   *kl,
			   *kr,
			   *kl1,
			   *kr1;
	uint32		f,
				r48l,
				r48r;
	int			round;

	if (count == 0)
		return (1);
	else if (count > 0)
	{
		 
		kl1 = en_keysl;
		kr1 = en_keysr;
	}
	else
	{
		 
		count = -count;
		kl1 = de_keysl;
		kr1 = de_keysr;
	}

	 
	l = ip_maskl[0][l_in >> 24]
		| ip_maskl[1][(l_in >> 16) & 0xff]
		| ip_maskl[2][(l_in >> 8) & 0xff]
		| ip_maskl[3][l_in & 0xff]
		| ip_maskl[4][r_in >> 24]
		| ip_maskl[5][(r_in >> 16) & 0xff]
		| ip_maskl[6][(r_in >> 8) & 0xff]
		| ip_maskl[7][r_in & 0xff];
	r = ip_maskr[0][l_in >> 24]
		| ip_maskr[1][(l_in >> 16) & 0xff]
		| ip_maskr[2][(l_in >> 8) & 0xff]
		| ip_maskr[3][l_in & 0xff]
		| ip_maskr[4][r_in >> 24]
		| ip_maskr[5][(r_in >> 16) & 0xff]
		| ip_maskr[6][(r_in >> 8) & 0xff]
		| ip_maskr[7][r_in & 0xff];

	while (count--)
	{
		 
		kl = kl1;
		kr = kr1;
		round = 16;
		while (round--)
		{
			 
			r48l = ((r & 0x00000001) << 23)
				| ((r & 0xf8000000) >> 9)
				| ((r & 0x1f800000) >> 11)
				| ((r & 0x01f80000) >> 13)
				| ((r & 0x001f8000) >> 15);

			r48r = ((r & 0x0001f800) << 7)
				| ((r & 0x00001f80) << 5)
				| ((r & 0x000001f8) << 3)
				| ((r & 0x0000001f) << 1)
				| ((r & 0x80000000) >> 31);

			 
			f = (r48l ^ r48r) & saltbits;
			r48l ^= f ^ *kl++;
			r48r ^= f ^ *kr++;

			 
			f = psbox[0][m_sbox[0][r48l >> 12]]
				| psbox[1][m_sbox[1][r48l & 0xfff]]
				| psbox[2][m_sbox[2][r48r >> 12]]
				| psbox[3][m_sbox[3][r48r & 0xfff]];

			 
			f ^= l;
			l = r;
			r = f;
		}
		r = l;
		l = f;
	}

	 
	*l_out = fp_maskl[0][l >> 24]
		| fp_maskl[1][(l >> 16) & 0xff]
		| fp_maskl[2][(l >> 8) & 0xff]
		| fp_maskl[3][l & 0xff]
		| fp_maskl[4][r >> 24]
		| fp_maskl[5][(r >> 16) & 0xff]
		| fp_maskl[6][(r >> 8) & 0xff]
		| fp_maskl[7][r & 0xff];
	*r_out = fp_maskr[0][l >> 24]
		| fp_maskr[1][(l >> 16) & 0xff]
		| fp_maskr[2][(l >> 8) & 0xff]
		| fp_maskr[3][l & 0xff]
		| fp_maskr[4][r >> 24]
		| fp_maskr[5][(r >> 16) & 0xff]
		| fp_maskr[6][(r >> 8) & 0xff]
		| fp_maskr[7][r & 0xff];
	return (0);
}
