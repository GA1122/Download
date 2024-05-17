void *CL_RefMallocDebug( int size, char *label, char *file, int line ) {
	return Z_TagMallocDebug( size, TAG_RENDERER, label, file, line );
}
