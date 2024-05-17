void opj_pi_update_encode_poc_and_final (   opj_cp_t *p_cp,
                                            OPJ_UINT32 p_tileno,
                                            OPJ_INT32 p_tx0,
                                            OPJ_INT32 p_tx1,
                                            OPJ_INT32 p_ty0,
                                            OPJ_INT32 p_ty1,
                                            OPJ_UINT32 p_max_prec,
                                            OPJ_UINT32 p_max_res,
                                            OPJ_UINT32 p_dx_min,
                                            OPJ_UINT32 p_dy_min)
{
	 
	OPJ_UINT32 pino;
	 
	opj_tcp_t *l_tcp = 00;
	 
	opj_poc_t * l_current_poc = 00;

	 
	OPJ_UINT32 l_poc_bound;

    OPJ_ARG_NOT_USED(p_max_res);

	 
	assert(p_cp != 00);
	assert(p_tileno < p_cp->tw * p_cp->th);

	 
	l_tcp = &p_cp->tcps [p_tileno];
	 
	l_poc_bound = l_tcp->numpocs+1;

	 
	l_current_poc = l_tcp->pocs;

	l_current_poc->compS = l_current_poc->compno0;
	l_current_poc->compE = l_current_poc->compno1;
	l_current_poc->resS = l_current_poc->resno0;
	l_current_poc->resE = l_current_poc->resno1;
	l_current_poc->layE = l_current_poc->layno1;

	 
	l_current_poc->layS = 0;
	l_current_poc->prg  = l_current_poc->prg1;
	l_current_poc->prcS = 0;

	l_current_poc->prcE = p_max_prec;
	l_current_poc->txS = (OPJ_UINT32)p_tx0;
	l_current_poc->txE = (OPJ_UINT32)p_tx1;
	l_current_poc->tyS = (OPJ_UINT32)p_ty0;
	l_current_poc->tyE = (OPJ_UINT32)p_ty1;
	l_current_poc->dx = p_dx_min;
	l_current_poc->dy = p_dy_min;

	++ l_current_poc;
	for (pino = 1;pino < l_poc_bound ; ++pino) {
		l_current_poc->compS = l_current_poc->compno0;
		l_current_poc->compE= l_current_poc->compno1;
		l_current_poc->resS = l_current_poc->resno0;
		l_current_poc->resE = l_current_poc->resno1;
		l_current_poc->layE = l_current_poc->layno1;
		l_current_poc->prg  = l_current_poc->prg1;
		l_current_poc->prcS = 0;
		 
		l_current_poc->layS = (l_current_poc->layE > (l_current_poc-1)->layE) ? l_current_poc->layE : 0;

		l_current_poc->prcE = p_max_prec;
		l_current_poc->txS = (OPJ_UINT32)p_tx0;
		l_current_poc->txE = (OPJ_UINT32)p_tx1;
		l_current_poc->tyS = (OPJ_UINT32)p_ty0;
		l_current_poc->tyE = (OPJ_UINT32)p_ty1;
		l_current_poc->dx = p_dx_min;
		l_current_poc->dy = p_dy_min;
		++ l_current_poc;
	}
}
