int jpc_putms(jas_stream_t *out, jpc_cstate_t *cstate, jpc_ms_t *ms)
{
	jas_stream_t *tmpstream;
	int len;

	 
	if (jpc_putuint16(out, ms->id)) {
		return -1;
	}

	 
	if (ms->ops->putparms) {
		 
		if (!(tmpstream = jas_stream_memopen(0, 0))) {
			return -1;
		}
		if ((*ms->ops->putparms)(ms, cstate, tmpstream)) {
			jas_stream_close(tmpstream);
			return -1;
		}
		 
		if ((len = jas_stream_tell(tmpstream)) < 0) {
			jas_stream_close(tmpstream);
			return -1;
		}
		ms->len = len;
		 
		if (jas_stream_seek(tmpstream, 0, SEEK_SET) < 0 ||
		  jpc_putuint16(out, ms->len + 2) ||
		  jas_stream_copy(out, tmpstream, ms->len) < 0) {
			jas_stream_close(tmpstream);
			return -1;
		}
		 
		jas_stream_close(tmpstream);
	}

	 
	if (ms->id == JPC_MS_SIZ) {
		cstate->numcomps = ms->parms.siz.numcomps;
	}

	if (jas_getdbglevel() > 0) {
		jpc_ms_dump(ms, stderr);
	}

	return 0;
}
