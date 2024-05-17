mm_malloc(struct mm_master *mm, size_t size)
{
	struct mm_share *mms, *tmp;

	if (size == 0)
		fatal("mm_malloc: try to allocate 0 space");
	if (size > SIZE_MAX - MM_MINSIZE + 1)
		fatal("mm_malloc: size too big");

	size = ((size + (MM_MINSIZE - 1)) / MM_MINSIZE) * MM_MINSIZE;

	RB_FOREACH(mms, mmtree, &mm->rb_free) {
		if (mms->size >= size)
			break;
	}

	if (mms == NULL)
		return (NULL);

	 
	memset(mms->address, 0xd0, size);

	tmp = mm_make_entry(mm, &mm->rb_allocated, mms->address, size);

	 
	mms->size -= size;
	mms->address = (char *)mms->address + size;

	if (mms->size == 0) {
		RB_REMOVE(mmtree, &mm->rb_free, mms);
		if (mm->mmalloc == NULL)
			free(mms);
		else
			mm_free(mm->mmalloc, mms);
	}

	return (tmp->address);
}