buf_add_txid(StringInfo buf, txid xid)
{
	TxidSnapshot *snap = (TxidSnapshot *) buf->data;

	 
	snap->nxip++;

	appendBinaryStringInfo(buf, (char *) &xid, sizeof(xid));
}
