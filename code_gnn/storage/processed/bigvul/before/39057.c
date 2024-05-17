txid_current(PG_FUNCTION_ARGS)
{
	txid		val;
	TxidEpoch	state;

	 
	PreventCommandDuringRecovery("txid_current()");

	load_xid_epoch(&state);

	val = convert_xid(GetTopTransactionId(), &state);

	PG_RETURN_INT64(val);
}
