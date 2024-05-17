static unsigned long write_object(struct sha1file *f,
				  struct object_entry *entry,
				  off_t write_offset)
{
	unsigned long limit, len;
	int usable_delta, to_reuse;

	if (!pack_to_stdout)
		crc32_begin(f);

	 
	if (!pack_size_limit || !nr_written)
		limit = 0;
	else if (pack_size_limit <= write_offset)
		 
		limit = 1;
	else
		limit = pack_size_limit - write_offset;

	if (!entry->delta)
		usable_delta = 0;	 
	else if (!pack_size_limit)
	       usable_delta = 1;	 
	else if (entry->delta->idx.offset == (off_t)-1)
		usable_delta = 0;	 
	else if (entry->delta->idx.offset)
		usable_delta = 1;	 
	else
		usable_delta = 0;	 

	if (!reuse_object)
		to_reuse = 0;	 
	else if (!entry->in_pack)
		to_reuse = 0;	 
	else if (entry->type == OBJ_REF_DELTA || entry->type == OBJ_OFS_DELTA)
				 
		to_reuse = usable_delta;
				 
	else if (entry->type != entry->in_pack_type)
		to_reuse = 0;	 
	else if (entry->delta)
		to_reuse = 0;	 
	else
		to_reuse = 1;	 

	if (!to_reuse)
		len = write_no_reuse_object(f, entry, limit, usable_delta);
	else
		len = write_reuse_object(f, entry, limit, usable_delta);
	if (!len)
		return 0;

	if (usable_delta)
		written_delta++;
	written++;
	if (!pack_to_stdout)
		entry->idx.crc32 = crc32_end(f);
	return len;
}
