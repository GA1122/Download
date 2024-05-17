static int ebt_buf_add_pad(struct ebt_entries_buf_state *state, unsigned int sz)
{
	char *b = state->buf_kern_start;

	BUG_ON(b && state->buf_kern_offset > state->buf_kern_len);

	if (b != NULL && sz > 0)
		memset(b + state->buf_kern_offset, 0, sz);
	 
	return ebt_buf_count(state, sz);
}
