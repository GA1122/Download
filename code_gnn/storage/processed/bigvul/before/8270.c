static struct reply_buffer *alloc_reply_buffer(
    XExtDisplayInfo *info,
    int nbytes)
{
    struct mem_cache_str *cache = (struct mem_cache_str *)info->data;
    struct reply_buffer *rbp;
    struct reply_buffer *saved_rb = NULL;
     
    for (rbp = cache->reply_buffers; rbp; rbp = rbp->next) {
	if (rbp->ref_count == 0) {
	    if (rbp->nbytes >= nbytes)
		return rbp;
	    else
		saved_rb = rbp;
	}
    }
    if (saved_rb) {
	saved_rb->buf = (unsigned char *)Xrealloc(saved_rb->buf, nbytes);
	if (!saved_rb->buf) {
	    saved_rb->nbytes = 0;
	    return NULL;
	}
	saved_rb->nbytes = nbytes;
	return saved_rb;
    }

     
    rbp = (struct reply_buffer *)Xmalloc(sizeof(struct reply_buffer));
    if (!rbp)
	return NULL;
    rbp->buf = (unsigned char *)Xmalloc(nbytes);
    if (!rbp->buf) {
	Xfree(rbp);
	return NULL;
    }
    rbp->nbytes = nbytes;
    rbp->ref_count = 0;
    rbp->next = cache->reply_buffers;
    cache->reply_buffers = rbp;
    return rbp;
}
