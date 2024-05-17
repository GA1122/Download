static int nfs4_setup_session_slot_tables(struct nfs4_session *ses)
{
	struct nfs4_slot_table *tbl;
	int status;

	dprintk("--> %s\n", __func__);
	 
	tbl = &ses->fc_slot_table;
	status = nfs4_realloc_slot_table(tbl, ses->fc_attrs.max_reqs, 1);
	if (status)  
		return status;
	 
	tbl = &ses->bc_slot_table;
	status = nfs4_realloc_slot_table(tbl, ses->bc_attrs.max_reqs, 0);
	if (status && tbl->slots == NULL)
		 
		nfs4_destroy_slot_tables(ses);
	return status;
}