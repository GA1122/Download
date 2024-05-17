void http_txn_reset_res(struct http_txn *txn)
{
	txn->rsp.flags = 0;
	txn->rsp.sol = txn->rsp.eol = txn->rsp.eoh = 0;  
	txn->rsp.next = 0;
	txn->rsp.chunk_len = 0LL;
	txn->rsp.body_len = 0LL;
	txn->rsp.msg_state = HTTP_MSG_RPBEFORE;  
}
