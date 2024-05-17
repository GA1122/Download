hashbin_t *hashbin_new(int type)
{
	hashbin_t* hashbin;

	 
	hashbin = kzalloc(sizeof(*hashbin), GFP_ATOMIC);
	if (!hashbin)
		return NULL;

	 
	hashbin->hb_type = type;
	hashbin->magic = HB_MAGIC;

	 
	if ( hashbin->hb_type & HB_LOCK ) {
		spin_lock_init(&hashbin->hb_spinlock);
	}

	return hashbin;
}
