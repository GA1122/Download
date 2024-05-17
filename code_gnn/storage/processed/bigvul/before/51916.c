AirPDcapStoreSa(
    PAIRPDCAP_CONTEXT ctx,
    AIRPDCAP_SEC_ASSOCIATION_ID *id)
{
    INT last_free;
    if (ctx->first_free_index>=AIRPDCAP_MAX_SEC_ASSOCIATIONS_NR) {
         
        return -1;
    }
    if (ctx->sa[ctx->first_free_index].used) {
         
         
         
        for (last_free=ctx->first_free_index; last_free<AIRPDCAP_MAX_SEC_ASSOCIATIONS_NR; last_free++)
            if (!ctx->sa[last_free].used)
                break;

        if (last_free>=AIRPDCAP_MAX_SEC_ASSOCIATIONS_NR) {
             
            return -1;
        }

         
        ctx->first_free_index=last_free;
    }

     
    ctx->index=ctx->first_free_index;

     
    memset(ctx->sa+ctx->index, 0, sizeof(AIRPDCAP_SEC_ASSOCIATION));

    ctx->sa[ctx->index].used=1;

     
    memcpy(&(ctx->sa[ctx->index].saId), id, sizeof(AIRPDCAP_SEC_ASSOCIATION_ID));

     
    ctx->first_free_index++;

     
    if (ctx->index > ctx->sa_index)
        ctx->sa_index=ctx->index;

    return ctx->index;
}
