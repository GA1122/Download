pkinit_fini_pkinit_oids(pkinit_plg_crypto_context ctx)
{
    if (ctx == NULL)
        return;

     
    if (--pkinit_oids_refs == 0)
        OBJ_cleanup();
}