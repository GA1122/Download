TPM_RC tpm_session_start_auth_with_params(TSS2_SYS_CONTEXT *sapi_context, SESSION **session,
    TPMI_DH_OBJECT tpmKey, TPM2B_MAX_BUFFER *salt,
    TPMI_DH_ENTITY bind, TPM2B_AUTH *bindAuth, TPM2B_NONCE *nonceCaller,
    TPM2B_ENCRYPTED_SECRET *encryptedSalt,
    TPM_SE sessionType, TPMT_SYM_DEF *symmetric, TPMI_ALG_HASH algId )
{
    TPM_RC rval;
    SESSION_LIST_ENTRY *sessionEntry;

    rval = AddSession( &sessionEntry );
    if( rval == TSS2_RC_SUCCESS )
    {
        *session = &sessionEntry->session;

        (*session)->bind = bind;
        (*session)->tpmKey = tpmKey;

        memcpy( &(*session)->nonceOlder.b, &nonceCaller->b, sizeof(nonceCaller->b));

        memcpy( &(*session)->encryptedSalt.b, &encryptedSalt->b, sizeof(encryptedSalt->b));

        (*session)->sessionType = sessionType;

        (*session)->symmetric.algorithm = symmetric->algorithm;
        (*session)->symmetric.keyBits.sym = symmetric->keyBits.sym;
        (*session)->symmetric.mode.sym = symmetric->mode.sym;
        (*session)->authHash = algId;

        if( bindAuth == 0 )
        {
            (*session)->authValueBind.b.size = 0;
        }
        else
        {
            memcpy( &( (*session)->authValueBind.b ), &( bindAuth->b ), sizeof(bindAuth->b));
        }

        if( (*session)->tpmKey == TPM_RH_NULL )
        {
            (*session)->salt.t.size = 0;
        }
        else
        {
            memcpy( &(*session)->salt.b, &salt->b, sizeof(salt->b));
        }

        if( (*session)->bind == TPM_RH_NULL )
            (*session)->authValueBind.t.size = 0;


        rval = StartAuthSession(sapi_context, *session );
    }
    else
    {
        DeleteSession( *session );
    }
    return( rval );
}
