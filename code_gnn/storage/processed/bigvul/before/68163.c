void* hashbin_remove_this( hashbin_t* hashbin, irda_queue_t* entry)
{
	unsigned long flags = 0;
	int	bin;
	long	hashv;

	IRDA_ASSERT( hashbin != NULL, return NULL;);
	IRDA_ASSERT( hashbin->magic == HB_MAGIC, return NULL;);
	IRDA_ASSERT( entry != NULL, return NULL;);

	 
	if ( hashbin->hb_type & HB_LOCK ) {
		spin_lock_irqsave(&hashbin->hb_spinlock, flags);
	}  

	 
	if((entry->q_next == NULL) || (entry->q_prev == NULL)) {
		entry = NULL;
		goto out;
	}

	 
	hashv = entry->q_hash;
	bin = GET_HASHBIN( hashv );

	 
	dequeue_general( (irda_queue_t**) &hashbin->hb_queue[ bin ],
			 entry);
	hashbin->hb_size--;
	entry->q_next = NULL;
	entry->q_prev = NULL;

	 
	if ( entry == hashbin->hb_current)
		hashbin->hb_current = NULL;
out:
	 
	if ( hashbin->hb_type & HB_LOCK ) {
		spin_unlock_irqrestore(&hashbin->hb_spinlock, flags);
	}  

	return entry;
}
