static int close_display(
    Display *dpy,
    XExtCodes *codes)		 
{
    XExtDisplayInfo *info = find_display (dpy);

    LockDisplay(dpy);
    if (info && info->data) {
	struct mem_cache_str *cache = (struct mem_cache_str *)info->data;
	struct intercept_queue *iq, *iq_next;
	struct reply_buffer *rbp, **rbp_next_p;

	for (iq=cache->inter_data; iq; iq=iq_next) {
	    iq_next = iq->next;
	    XFree(iq);
	    cache->inter_data_count--;
	}

	 
	for (rbp_next_p = &cache->reply_buffers; *rbp_next_p; ) {
	    rbp = *rbp_next_p;
	    if (rbp->ref_count == 0) {
		*rbp_next_p = rbp->next;
		XFree(rbp->buf);
		XFree(rbp);
	    } else {
		rbp_next_p = &rbp->next;
	    }
	}

	if (cache->reply_buffers == NULL  &&  cache->inter_data_count == 0) {
	     
	    XFree(cache);
	} else {
	    cache->display_closed = True;
	    cache->inter_data = NULL;  
	}
    }
    UnlockDisplay(dpy);
    return XextRemoveDisplay(xrecord_info, dpy);
}
