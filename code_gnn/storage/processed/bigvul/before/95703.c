void *CL_RefMalloc( int size ) {
	return Z_TagMalloc( size, TAG_RENDERER );
}
