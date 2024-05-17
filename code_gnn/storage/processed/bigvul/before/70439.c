static int jpc_dec_process_sot(jpc_dec_t *dec, jpc_ms_t *ms)
{
	jpc_dec_tile_t *tile;
	jpc_sot_t *sot = &ms->parms.sot;
	jas_image_cmptparm_t *compinfos;
	jas_image_cmptparm_t *compinfo;
	jpc_dec_cmpt_t *cmpt;
	int cmptno;

	if (dec->state == JPC_MH) {

		if (!(compinfos = jas_alloc2(dec->numcomps,
		  sizeof(jas_image_cmptparm_t)))) {
			abort();
		}
		for (cmptno = 0, cmpt = dec->cmpts, compinfo = compinfos;
		  cmptno < dec->numcomps; ++cmptno, ++cmpt, ++compinfo) {
			compinfo->tlx = 0;
			compinfo->tly = 0;
			compinfo->prec = cmpt->prec;
			compinfo->sgnd = cmpt->sgnd;
			compinfo->width = cmpt->width;
			compinfo->height = cmpt->height;
			compinfo->hstep = cmpt->hstep;
			compinfo->vstep = cmpt->vstep;
		}

		if (!(dec->image = jas_image_create(dec->numcomps, compinfos,
		  JAS_CLRSPC_UNKNOWN))) {
			jas_free(compinfos);
			return -1;
		}
		jas_free(compinfos);

		 
		if (dec->ppmstab) {
			 
			if (!(dec->pkthdrstreams = jpc_ppmstabtostreams(dec->ppmstab))) {
				abort();
			}
			jpc_ppxstab_destroy(dec->ppmstab);
			dec->ppmstab = 0;
		}
	}

	if (sot->len > 0) {
		dec->curtileendoff = jas_stream_getrwcount(dec->in) - ms->len -
		  4 + sot->len;
	} else {
		dec->curtileendoff = 0;
	}

	if (JAS_CAST(int, sot->tileno) >= dec->numtiles) {
		jas_eprintf("invalid tile number in SOT marker segment\n");
		return -1;
	}
	 
	dec->curtile = &dec->tiles[sot->tileno];
	tile = dec->curtile;
	 
	if (sot->partno != tile->partno) {
		return -1;
	}
	if (tile->numparts > 0 && sot->partno >= tile->numparts) {
		return -1;
	}
	if (!tile->numparts && sot->numparts > 0) {
		tile->numparts = sot->numparts;
	}

	tile->pptstab = 0;

	switch (tile->state) {
	case JPC_TILE_INIT:
		 
		tile->state = JPC_TILE_ACTIVE;
		assert(!tile->cp);
		if (!(tile->cp = jpc_dec_cp_copy(dec->cp))) {
			return -1;
		}
		jpc_dec_cp_resetflags(dec->cp);
		break;
	default:
		if (sot->numparts == sot->partno - 1) {
			tile->state = JPC_TILE_ACTIVELAST;
		}
		break;
	}

	 

	 
	dec->state = JPC_TPH;

	return 0;
}
