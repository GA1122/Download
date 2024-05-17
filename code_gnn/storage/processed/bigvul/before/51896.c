AirPDcapCleanSecAssoc(
    PAIRPDCAP_CONTEXT ctx)
{
    PAIRPDCAP_SEC_ASSOCIATION psa;
    int i;

    for (psa = ctx->sa, i = 0; i < AIRPDCAP_MAX_SEC_ASSOCIATIONS_NR; i++, psa++) {
         
        AirPDcapRecurseCleanSA(psa);
    }
}
