XRecordFreeData(XRecordInterceptData *data)
{
     
    struct intercept_queue *iq = (struct intercept_queue *)data;
    struct reply_buffer *rbp = NULL;
    struct mem_cache_str *cache = iq->cache;

     
    if (data->data) {

	for (rbp = cache->reply_buffers; rbp; rbp = rbp->next) {
	    if (data->data >= rbp->buf
		&& data->data < rbp->buf + rbp->nbytes)
	    {
		assert(rbp->ref_count > 0);
		rbp->ref_count--;
		break;
	    }
	}
	 
	assert(rbp);
    }
     
    if (cache->display_closed == False) {
	iq->next = cache->inter_data;
	cache->inter_data = iq;
    } else {
	if (rbp && rbp->ref_count == 0) {
	    struct reply_buffer *rbp2, **rbp_next_p;

	     
	    for (rbp_next_p = &cache->reply_buffers; *rbp_next_p; ) {
		rbp2 = *rbp_next_p;
		if (rbp == rbp2) {
		    *rbp_next_p = rbp2->next;
		    break;
		} else {
		    rbp_next_p = &rbp2->next;
		}
	    }
	    XFree(rbp->buf);
	    XFree(rbp);
	}

	XFree(iq);
	cache->inter_data_count--;

	if (cache->reply_buffers == NULL  &&  cache->inter_data_count == 0) {
	    XFree(cache);  
	}
    }
}
