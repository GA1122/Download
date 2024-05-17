static int jpc_sop_getparms(jpc_ms_t *ms, jpc_cstate_t *cstate, jas_stream_t *in)
{
	jpc_sop_t *sop = &ms->parms.sop;

	 
	cstate = 0;

	if (jpc_getuint16(in, &sop->seqno)) {
		return -1;
	}
	return 0;
}
