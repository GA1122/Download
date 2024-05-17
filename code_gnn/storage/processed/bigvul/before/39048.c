buf_finalize(StringInfo buf)
{
	TxidSnapshot *snap = (TxidSnapshot *) buf->data;

	SET_VARSIZE(snap, buf->len);

	 
	buf->data = NULL;
	pfree(buf);

	return snap;
}
