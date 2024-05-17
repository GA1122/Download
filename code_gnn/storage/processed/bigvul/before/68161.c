void* hashbin_remove( hashbin_t* hashbin, long hashv, const char* name)
{
	int bin, found = FALSE;
	unsigned long flags = 0;
	irda_queue_t* entry;

	IRDA_ASSERT( hashbin != NULL, return NULL;);
	IRDA_ASSERT( hashbin->magic == HB_MAGIC, return NULL;);

	 
	if ( name )
		hashv = hash( name );
	bin = GET_HASHBIN( hashv );

	 
	if ( hashbin->hb_type & HB_LOCK ) {
		spin_lock_irqsave(&hashbin->hb_spinlock, flags);
	}  

	 
	entry = hashbin->hb_queue[ bin ];
	if ( entry ) {
		do {
			 
			if ( entry->q_hash == hashv ) {
				 
				if ( name ) {
					if ( strcmp( entry->q_name, name) == 0)
					{
						found = TRUE;
						break;
					}
				} else {
					found = TRUE;
					break;
				}
			}
			entry = entry->q_next;
		} while ( entry != hashbin->hb_queue[ bin ] );
	}

	 
	if ( found ) {
		dequeue_general( (irda_queue_t**) &hashbin->hb_queue[ bin ],
				 entry);
		hashbin->hb_size--;

		 
		if ( entry == hashbin->hb_current)
			hashbin->hb_current = NULL;
	}

	 
	if ( hashbin->hb_type & HB_LOCK ) {
		spin_unlock_irqrestore(&hashbin->hb_spinlock, flags);
	}  


	 
	if ( found )
		return entry;
	else
		return NULL;

}
