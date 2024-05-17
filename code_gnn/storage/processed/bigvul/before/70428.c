static int jpc_dec_process_crg(jpc_dec_t *dec, jpc_ms_t *ms)
{
	int cmptno;
	jpc_dec_cmpt_t *cmpt;
	jpc_crg_t *crg;

	crg = &ms->parms.crg;
	for (cmptno = 0, cmpt = dec->cmpts; cmptno < dec->numcomps; ++cmptno,
	  ++cmpt) {
		 
	}
	return 0;
}
