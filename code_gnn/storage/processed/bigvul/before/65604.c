static void nfsd4_init_conn(struct svc_rqst *rqstp, struct nfsd4_conn *conn, struct nfsd4_session *ses)
{
	int ret;

	nfsd4_hash_conn(conn, ses);
	ret = nfsd4_register_conn(conn);
	if (ret)
		 
		nfsd4_conn_lost(&conn->cn_xpt_user);
	 
	nfsd4_probe_callback_sync(ses->se_client);
}