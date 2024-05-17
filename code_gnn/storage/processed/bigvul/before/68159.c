void* hashbin_lock_find( hashbin_t* hashbin, long hashv, const char* name )
{
	unsigned long flags = 0;
	irda_queue_t* entry;

	 
	spin_lock_irqsave(&hashbin->hb_spinlock, flags);

	 
	entry = hashbin_find(hashbin, hashv, name);

	 
	spin_unlock_irqrestore(&hashbin->hb_spinlock, flags);

	return entry;
}
