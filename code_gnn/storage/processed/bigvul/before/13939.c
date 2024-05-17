gs_currentnamedicc(const gs_gstate * pgs, gs_param_string * pval)
{
    static const char *const rfs = "";

    if (pgs->icc_manager->device_named == NULL) {
        pval->data = (const byte *) rfs;
        pval->persistent = true;
    } else {
        pval->data = (const byte *) (pgs->icc_manager->device_named->name);
        pval->persistent = false;
    }
    pval->size = strlen((const char *)pval->data);
}
