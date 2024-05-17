void http_return_srv_error(struct session *s, struct stream_interface *si)
{
	int err_type = si->err_type;

	if (err_type & SI_ET_QUEUE_ABRT)
		http_server_error(s, si, SN_ERR_CLICL, SN_FINST_Q,
				  503, http_error_message(s, HTTP_ERR_503));
	else if (err_type & SI_ET_CONN_ABRT)
		http_server_error(s, si, SN_ERR_CLICL, SN_FINST_C,
				  503, (s->txn.flags & TX_NOT_FIRST) ? NULL :
				  http_error_message(s, HTTP_ERR_503));
	else if (err_type & SI_ET_QUEUE_TO)
		http_server_error(s, si, SN_ERR_SRVTO, SN_FINST_Q,
				  503, http_error_message(s, HTTP_ERR_503));
	else if (err_type & SI_ET_QUEUE_ERR)
		http_server_error(s, si, SN_ERR_SRVCL, SN_FINST_Q,
				  503, http_error_message(s, HTTP_ERR_503));
	else if (err_type & SI_ET_CONN_TO)
		http_server_error(s, si, SN_ERR_SRVTO, SN_FINST_C,
				  503, (s->txn.flags & TX_NOT_FIRST) ? NULL :
				  http_error_message(s, HTTP_ERR_503));
	else if (err_type & SI_ET_CONN_ERR)
		http_server_error(s, si, SN_ERR_SRVCL, SN_FINST_C,
				  503, (s->flags & SN_SRV_REUSED) ? NULL :
				  http_error_message(s, HTTP_ERR_503));
	else if (err_type & SI_ET_CONN_RES)
		http_server_error(s, si, SN_ERR_RESOURCE, SN_FINST_C,
				  503, (s->txn.flags & TX_NOT_FIRST) ? NULL :
				  http_error_message(s, HTTP_ERR_503));
	else  
		http_server_error(s, si, SN_ERR_INTERNAL, SN_FINST_C,
				  500, http_error_message(s, HTTP_ERR_500));
}