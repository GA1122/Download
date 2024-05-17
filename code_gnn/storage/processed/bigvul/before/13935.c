gs_currentdefaultgrayicc(const gs_gstate * pgs, gs_param_string * pval)
{
    static const char *const rfs = DEFAULT_GRAY_ICC;

    if (pgs->icc_manager->default_gray == NULL) {
        pval->data = (const byte *) rfs;
        pval->persistent = true;
    } else {
        pval->data = (const byte *) (pgs->icc_manager->default_gray->name);
        pval->persistent = false;
    }
    pval->size = strlen((const char *)pval->data);
}
