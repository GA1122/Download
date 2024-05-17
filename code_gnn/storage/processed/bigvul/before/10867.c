des_init(void)
{
	int			i,
				j,
				b,
				k,
				inbit,
				obit;
	uint32	   *p,
			   *il,
			   *ir,
			   *fl,
			   *fr;

	old_rawkey0 = old_rawkey1 = 0L;
	saltbits = 0L;
	old_salt = 0L;
	bits24 = (bits28 = _crypt_bits32 + 4) + 4;

	 
	for (i = 0; i < 8; i++)
		for (j = 0; j < 64; j++)
		{
			b = (j & 0x20) | ((j & 1) << 4) | ((j >> 1) & 0xf);
			u_sbox[i][j] = sbox[i][b];
		}

	 
	for (b = 0; b < 4; b++)
		for (i = 0; i < 64; i++)
			for (j = 0; j < 64; j++)
				m_sbox[b][(i << 6) | j] =
					(u_sbox[(b << 1)][i] << 4) |
					u_sbox[(b << 1) + 1][j];

	 
	for (i = 0; i < 64; i++)
	{
		init_perm[final_perm[i] = IP[i] - 1] = i;
		inv_key_perm[i] = 255;
	}

	 
	for (i = 0; i < 56; i++)
	{
		u_key_perm[i] = key_perm[i] - 1;
		inv_key_perm[key_perm[i] - 1] = i;
		inv_comp_perm[i] = 255;
	}

	 
	for (i = 0; i < 48; i++)
		inv_comp_perm[comp_perm[i] - 1] = i;

	 
	for (k = 0; k < 8; k++)
	{
		for (i = 0; i < 256; i++)
		{
			*(il = &ip_maskl[k][i]) = 0L;
			*(ir = &ip_maskr[k][i]) = 0L;
			*(fl = &fp_maskl[k][i]) = 0L;
			*(fr = &fp_maskr[k][i]) = 0L;
			for (j = 0; j < 8; j++)
			{
				inbit = 8 * k + j;
				if (i & _crypt_bits8[j])
				{
					if ((obit = init_perm[inbit]) < 32)
						*il |= _crypt_bits32[obit];
					else
						*ir |= _crypt_bits32[obit - 32];
					if ((obit = final_perm[inbit]) < 32)
						*fl |= _crypt_bits32[obit];
					else
						*fr |= _crypt_bits32[obit - 32];
				}
			}
		}
		for (i = 0; i < 128; i++)
		{
			*(il = &key_perm_maskl[k][i]) = 0L;
			*(ir = &key_perm_maskr[k][i]) = 0L;
			for (j = 0; j < 7; j++)
			{
				inbit = 8 * k + j;
				if (i & _crypt_bits8[j + 1])
				{
					if ((obit = inv_key_perm[inbit]) == 255)
						continue;
					if (obit < 28)
						*il |= bits28[obit];
					else
						*ir |= bits28[obit - 28];
				}
			}
			*(il = &comp_maskl[k][i]) = 0L;
			*(ir = &comp_maskr[k][i]) = 0L;
			for (j = 0; j < 7; j++)
			{
				inbit = 7 * k + j;
				if (i & _crypt_bits8[j + 1])
				{
					if ((obit = inv_comp_perm[inbit]) == 255)
						continue;
					if (obit < 24)
						*il |= bits24[obit];
					else
						*ir |= bits24[obit - 24];
				}
			}
		}
	}

	 
	for (i = 0; i < 32; i++)
		un_pbox[pbox[i] - 1] = i;

	for (b = 0; b < 4; b++)
		for (i = 0; i < 256; i++)
		{
			*(p = &psbox[b][i]) = 0L;
			for (j = 0; j < 8; j++)
			{
				if (i & _crypt_bits8[j])
					*p |= _crypt_bits32[un_pbox[8 * b + j]];
			}
		}

	des_initialised = 1;
}