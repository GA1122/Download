clear_config()
{
	register 	int 	i;
	register 	BUCKET	*ptr = NULL;
	register 	BUCKET	*tmp = NULL;

	for( i=0; i<TABLESIZE; i++ ) {
		ptr = ConfigTab[i];
		while( ptr ) {
			tmp = ptr->next;
			FREE( ptr->value );
			ptr->value = NULL;
			FREE( ptr->name );
			ptr->name = NULL;
			FREE( ptr );
			ptr = tmp;
		}
		ConfigTab[i] = NULL;
	}
	if (extra_info != NULL) {
		delete extra_info;
		extra_info = NULL;
	}
	global_config_source       = "";
	local_config_sources.clearAll();
	return;
}
