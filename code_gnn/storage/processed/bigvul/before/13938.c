gs_currentlabicc(const gs_gstate * pgs, gs_param_string * pval)
{
    static const char *const rfs = LAB_ICC;

    pval->data = (const byte *)( (pgs->icc_manager->lab_profile == NULL) ?
                        rfs : pgs->icc_manager->lab_profile->name);
    pval->size = strlen((const char *)pval->data);
    pval->persistent = true;
}
