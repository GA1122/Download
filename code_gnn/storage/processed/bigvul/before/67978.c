_kdc_tgs_rep(krb5_context context,
	     krb5_kdc_configuration *config,
	     KDC_REQ *req,
	     krb5_data *data,
	     const char *from,
	     struct sockaddr *from_addr,
	     int datagram_reply)
{
    AuthorizationData *auth_data = NULL;
    krb5_error_code ret;
    int i = 0;
    const PA_DATA *tgs_req;

    hdb_entry_ex *krbtgt = NULL;
    krb5_ticket *ticket = NULL;
    const char *e_text = NULL;
    krb5_enctype krbtgt_etype = ETYPE_NULL;

    krb5_keyblock *replykey = NULL;
    int rk_is_subkey = 0;
    time_t *csec = NULL;
    int *cusec = NULL;

    if(req->padata == NULL){
	ret = KRB5KDC_ERR_PREAUTH_REQUIRED;  
	kdc_log(context, config, 0,
		"TGS-REQ from %s without PA-DATA", from);
	goto out;
    }

    tgs_req = _kdc_find_padata(req, &i, KRB5_PADATA_TGS_REQ);

    if(tgs_req == NULL){
	ret = KRB5KDC_ERR_PADATA_TYPE_NOSUPP;

	kdc_log(context, config, 0,
		"TGS-REQ from %s without PA-TGS-REQ", from);
	goto out;
    }
    ret = tgs_parse_request(context, config,
			    &req->req_body, tgs_req,
			    &krbtgt,
			    &krbtgt_etype,
			    &ticket,
			    &e_text,
			    from, from_addr,
			    &csec, &cusec,
			    &auth_data,
			    &replykey,
			    &rk_is_subkey);
    if (ret == HDB_ERR_NOT_FOUND_HERE) {
	 
	goto out;
    }
    if (ret) {
	kdc_log(context, config, 0,
		"Failed parsing TGS-REQ from %s", from);
	goto out;
    }

    {
	const PA_DATA *pa = _kdc_find_padata(req, &i, KRB5_PADATA_FX_FAST);
	if (pa)
	    kdc_log(context, config, 10, "Got TGS FAST request"); 
    }


    ret = tgs_build_reply(context,
			  config,
			  req,
			  &req->req_body,
			  krbtgt,
			  krbtgt_etype,
			  replykey,
			  rk_is_subkey,
			  ticket,
			  data,
			  from,
			  &e_text,
			  &auth_data,
			  from_addr);
    if (ret) {
	kdc_log(context, config, 0,
		"Failed building TGS-REP to %s", from);
	goto out;
    }

     
    if (datagram_reply && data->length > config->max_datagram_reply_length) {
	krb5_data_free(data);
	ret = KRB5KRB_ERR_RESPONSE_TOO_BIG;
	e_text = "Reply packet too large";
    }

out:
    if (replykey)
	krb5_free_keyblock(context, replykey);

    if(ret && ret != HDB_ERR_NOT_FOUND_HERE && data->data == NULL){
	 
	METHOD_DATA error_method = { 0, NULL };
	

	kdc_log(context, config, 10, "tgs-req: sending error: %d to client", ret);
	ret = _kdc_fast_mk_error(context, NULL,
				 &error_method,
				 NULL,
				 NULL,
				 ret, NULL,
				 NULL,
				 NULL, NULL,
				 csec, cusec,
				 data);
	free_METHOD_DATA(&error_method);
    }
    free(csec);
    free(cusec);
    if (ticket)
	krb5_free_ticket(context, ticket);
    if(krbtgt)
	_kdc_free_ent(context, krbtgt);

    if (auth_data) {
	free_AuthorizationData(auth_data);
	free(auth_data);
    }

    return ret;
}