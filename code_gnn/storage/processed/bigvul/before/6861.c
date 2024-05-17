void http_txn_reset_req(struct http_txn *txn)
{
	txn->req.flags = 0;
	txn->req.sol = txn->req.eol = txn->req.eoh = 0;  
	txn->req.next = 0;
	txn->req.chunk_len = 0LL;
	txn->req.body_len = 0LL;
	txn->req.msg_state = HTTP_MSG_RQBEFORE;  
}
