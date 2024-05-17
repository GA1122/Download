AirPDcapGetSaPtr(
    PAIRPDCAP_CONTEXT ctx,
    AIRPDCAP_SEC_ASSOCIATION_ID *id)
{
    int sa_index;

     
    if ((sa_index=AirPDcapGetSa(ctx, id))==-1) {
         
        if ((sa_index=AirPDcapStoreSa(ctx, id))==-1) {
            return NULL;
        }
    }
     
    return &ctx->sa[sa_index];
}
