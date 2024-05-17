convert_xid(TransactionId xid, const TxidEpoch *state)
{
	uint64		epoch;

	 
	if (!TransactionIdIsNormal(xid))
		return (txid) xid;

	 
	epoch = (uint64) state->epoch;
	if (xid > state->last_xid &&
		TransactionIdPrecedes(xid, state->last_xid))
		epoch--;
	else if (xid < state->last_xid &&
			 TransactionIdFollows(xid, state->last_xid))
		epoch++;

	return (epoch << 32) | xid;
}
