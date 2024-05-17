static enum write_one_status write_one(struct sha1file *f,
				       struct object_entry *e,
				       off_t *offset)
{
	unsigned long size;
	int recursing;

	 
	recursing = (e->idx.offset == 1);
	if (recursing) {
		warning("recursive delta detected for object %s",
			sha1_to_hex(e->idx.sha1));
		return WRITE_ONE_RECURSIVE;
	} else if (e->idx.offset || e->preferred_base) {
		 
		return WRITE_ONE_SKIP;
	}

	 
	if (e->delta) {
		e->idx.offset = 1;  
		switch (write_one(f, e->delta, offset)) {
		case WRITE_ONE_RECURSIVE:
			 
			e->delta = NULL;
			break;
		default:
			break;
		case WRITE_ONE_BREAK:
			e->idx.offset = recursing;
			return WRITE_ONE_BREAK;
		}
	}

	e->idx.offset = *offset;
	size = write_object(f, e, *offset);
	if (!size) {
		e->idx.offset = recursing;
		return WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	 
	if (signed_add_overflows(*offset, size))
		die("pack too large for current definition of off_t");
	*offset += size;
	return WRITE_ONE_WRITTEN;
}
