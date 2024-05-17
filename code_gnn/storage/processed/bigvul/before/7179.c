int ssl3_cbc_remove_padding(const SSL* s,
			    SSL3_RECORD *rec,
			    unsigned block_size,
			    unsigned mac_size)
	{
	unsigned padding_length, good;
	const unsigned overhead = 1   + mac_size;

	 
	if (overhead > rec->length)
		return 0;

	padding_length = rec->data[rec->length-1];
	good = constant_time_ge(rec->length, padding_length+overhead);
	 
	good &= constant_time_ge(block_size, padding_length+1);
	padding_length = good & (padding_length+1);
	rec->length -= padding_length;
	rec->type |= padding_length<<8;	 
	return (int)((good & 1) | (~good & -1));
}
