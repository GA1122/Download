AirPDcapGetSa(
    PAIRPDCAP_CONTEXT ctx,
    AIRPDCAP_SEC_ASSOCIATION_ID *id)
{
    INT sa_index;
    if (ctx->sa_index!=-1) {
         
         
        for (sa_index=ctx->sa_index; sa_index>=0; sa_index--) {
            if (ctx->sa[sa_index].used) {
                if (memcmp(id, &(ctx->sa[sa_index].saId), sizeof(AIRPDCAP_SEC_ASSOCIATION_ID))==0) {
                    ctx->index=sa_index;
                    return sa_index;
                }
            }
        }
    }

    return -1;
}