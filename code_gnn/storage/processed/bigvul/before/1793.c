getcode(CompressedFile *file)
{
    register code_int code;
    register int r_off, bits;
    register char_type *bp = file->buf;
    register BufFilePtr	raw;

    if ( file->clear_flg > 0 || file->offset >= file->size ||
	file->free_ent > file->maxcode )
    {
	 
	if ( file->free_ent > file->maxcode ) {
	    file->n_bits++;
	    if ( file->n_bits == file->maxbits )
		file->maxcode = file->maxmaxcode;	 
	    else
		file->maxcode = MAXCODE(file->n_bits);
	}
	if ( file->clear_flg > 0) {
    	    file->maxcode = MAXCODE (file->n_bits = INIT_BITS);
	    file->clear_flg = 0;
	}
	bits = file->n_bits;
	raw = file->file;
	while (bits > 0 && (code = BufFileGet (raw)) != BUFFILEEOF)
	{
	    *bp++ = code;
	    --bits;
	}
	bp = file->buf;
	if (bits == file->n_bits)
	    return -1;			 
	file->size = file->n_bits - bits;
	file->offset = 0;
	 
	file->size = (file->size << 3) - (file->n_bits - 1);
    }
    r_off = file->offset;
    bits = file->n_bits;
     
    bp += (r_off >> 3);
    r_off &= 7;
     
#ifdef NO_UCHAR
    code = ((*bp++ >> r_off) & rmask[8 - r_off]) & 0xff;
#else
    code = (*bp++ >> r_off);
#endif  
    bits -= (8 - r_off);
    r_off = 8 - r_off;		 
     
    if ( bits >= 8 ) {
#ifdef NO_UCHAR
	code |= (*bp++ & 0xff) << r_off;
#else
	code |= *bp++ << r_off;
#endif  
	r_off += 8;
	bits -= 8;
    }
     
    code |= (*bp & rmask[bits]) << r_off;
    file->offset += file->n_bits;

    return code;
}