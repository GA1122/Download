static void FS_ReorderPurePaks( void )
{
	searchpath_t *s;
	int i;
	searchpath_t **p_insert_index,  
		**p_previous;  

	fs_reordered = qfalse;

	if ( !fs_numServerPaks )
		return;

	p_insert_index = &fs_searchpaths;  
	for ( i = 0 ; i < fs_numServerPaks ; i++ ) {
		p_previous = p_insert_index;  
		for (s = *p_insert_index; s; s = s->next) {
			if (s->pack && fs_serverPaks[i] == s->pack->checksum) {
				fs_reordered = qtrue;
				*p_previous = s->next;
				s->next = *p_insert_index;
				*p_insert_index = s;
				p_insert_index = &s->next;
				break;  
			}
			p_previous = &s->next;
		}
	}
}
