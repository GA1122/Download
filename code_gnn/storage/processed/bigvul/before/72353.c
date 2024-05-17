process_remove_all_identities(SocketEntry *e, int version)
{
	Idtab *tab = idtab_lookup(version);
	Identity *id;

	 
	for (id = TAILQ_FIRST(&tab->idlist); id;
	    id = TAILQ_FIRST(&tab->idlist)) {
		TAILQ_REMOVE(&tab->idlist, id, next);
		free_identity(id);
	}

	 
	tab->nentries = 0;

	 
	send_status(e, 1);
}
