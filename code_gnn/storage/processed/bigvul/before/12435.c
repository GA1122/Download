static void spl_object_storage_free_hash(spl_SplObjectStorage *intern, char *hash) {
	if (intern->fptr_get_hash) {
		efree(hash);
	} else {
#if HAVE_PACKED_OBJECT_VALUE
		 
#else
		efree(hash);
#endif
	}
}
