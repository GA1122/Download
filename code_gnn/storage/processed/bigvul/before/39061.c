txid_snapshot_xip(PG_FUNCTION_ARGS)
{
	FuncCallContext *fctx;
	TxidSnapshot *snap;
	txid		value;

	 
	if (SRF_IS_FIRSTCALL())
	{
		TxidSnapshot *arg = (TxidSnapshot *) PG_GETARG_VARLENA_P(0);

		fctx = SRF_FIRSTCALL_INIT();

		 
		snap = MemoryContextAlloc(fctx->multi_call_memory_ctx, VARSIZE(arg));
		memcpy(snap, arg, VARSIZE(arg));

		fctx->user_fctx = snap;
	}

	 
	fctx = SRF_PERCALL_SETUP();
	snap = fctx->user_fctx;
	if (fctx->call_cntr < snap->nxip)
	{
		value = snap->xip[fctx->call_cntr];
		SRF_RETURN_NEXT(fctx, Int64GetDatum(value));
	}
	else
	{
		SRF_RETURN_DONE(fctx);
	}
}
