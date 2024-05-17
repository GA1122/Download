static int reset_saslconn(sasl_conn_t **conn)
{
    int ret;
    sasl_security_properties_t *secprops = NULL;

    sasl_dispose(conn);
     
    ret = sasl_server_new("imap", config_servername,
                          NULL, NULL, NULL,
                          NULL, 0, conn);
    if(ret != SASL_OK) return ret;

    if(saslprops.ipremoteport)
        ret = sasl_setprop(*conn, SASL_IPREMOTEPORT,
                           saslprops.ipremoteport);
    if(ret != SASL_OK) return ret;

    if(saslprops.iplocalport)
        ret = sasl_setprop(*conn, SASL_IPLOCALPORT,
                           saslprops.iplocalport);
    if(ret != SASL_OK) return ret;

    secprops = mysasl_secprops(0);
    ret = sasl_setprop(*conn, SASL_SEC_PROPS, secprops);
    if(ret != SASL_OK) return ret;
     

     
    if(saslprops.ssf) {
        ret = sasl_setprop(*conn, SASL_SSF_EXTERNAL, &saslprops.ssf);
    } else {
        ret = sasl_setprop(*conn, SASL_SSF_EXTERNAL, &extprops_ssf);
    }
    if(ret != SASL_OK) return ret;

    if(saslprops.authid) {
        ret = sasl_setprop(*conn, SASL_AUTH_EXTERNAL, saslprops.authid);
        if(ret != SASL_OK) return ret;
    }
     

    return SASL_OK;
}
