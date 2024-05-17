get_ticket_flags(krb5_flags reqflags, krb5_db_entry *client,
                 krb5_db_entry *server, krb5_enc_tkt_part *header_enc)
{
    krb5_flags flags;

     
    flags = OPTS2FLAGS(reqflags) | TKT_FLG_ENC_PA_REP;
    if (reqflags & KDC_OPT_POSTDATED)
        flags |= TKT_FLG_INVALID;
    if (header_enc != NULL)
        flags |= COPY_TKT_FLAGS(header_enc->flags);
    if (header_enc == NULL)
        flags |= TKT_FLG_INITIAL;

     
    if (header_enc != NULL && (server->attributes & KRB5_KDB_OK_AS_DELEGATE))
        flags |= TKT_FLG_OK_AS_DELEGATE;

     
    if (client != NULL && (client->attributes & KRB5_KDB_DISALLOW_PROXIABLE))
        flags &= ~TKT_FLG_PROXIABLE;
    if (server->attributes & KRB5_KDB_DISALLOW_PROXIABLE)
        flags &= ~TKT_FLG_PROXIABLE;
    if (header_enc != NULL && !(header_enc->flags & TKT_FLG_PROXIABLE))
        flags &= ~TKT_FLG_PROXIABLE;

     
    if (client != NULL && (client->attributes & KRB5_KDB_DISALLOW_FORWARDABLE))
        flags &= ~TKT_FLG_FORWARDABLE;
    if (server->attributes & KRB5_KDB_DISALLOW_FORWARDABLE)
        flags &= ~TKT_FLG_FORWARDABLE;
    if (header_enc != NULL && !(header_enc->flags & TKT_FLG_FORWARDABLE))
        flags &= ~TKT_FLG_FORWARDABLE;

     
    if (header_enc != NULL && !(header_enc->flags & TKT_FLG_ANONYMOUS))
        flags &= ~TKT_FLG_ANONYMOUS;

    return flags;
}