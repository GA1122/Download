static void end_read(TsHashTable *ht)
{
#ifdef ZTS
	tsrm_mutex_lock(ht->mx_reader);
	if ((--(ht->reader)) == 0) {
		tsrm_mutex_unlock(ht->mx_writer);
	}
	tsrm_mutex_unlock(ht->mx_reader);
#endif
}