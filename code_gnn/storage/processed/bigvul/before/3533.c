int buffer_insert_line2(struct buffer *b, char *pos, const char *str, int len)
{
	int delta;

	delta = len + 2;

	if (bi_end(b) + delta >= b->data + b->size)
		return 0;   

	if (buffer_not_empty(b) &&
	    bi_end(b) + delta > bo_ptr(b) &&
	    bo_ptr(b) >= bi_end(b))
		return 0;   

	 
	memmove(pos + delta, pos, bi_end(b) - pos);

	 
	if (len && str) {
		memcpy(pos, str, len);
		pos[len] = '\r';
		pos[len + 1] = '\n';
	}

	b->i += delta;
        return delta;
 }
