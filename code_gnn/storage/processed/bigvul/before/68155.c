void* hashbin_find_next( hashbin_t* hashbin, long hashv, const char* name,
			 void ** pnext)
{
	unsigned long flags = 0;
	irda_queue_t* entry;

	 
	spin_lock_irqsave(&hashbin->hb_spinlock, flags);

	 
	entry = hashbin_find(hashbin, hashv, name);

	 
	if(entry) {
		hashbin->hb_current = entry;
		*pnext = hashbin_get_next( hashbin );
	} else
		*pnext = NULL;

	 
	spin_unlock_irqrestore(&hashbin->hb_spinlock, flags);

	return entry;
}
