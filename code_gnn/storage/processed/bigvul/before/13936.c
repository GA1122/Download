gs_currentdevicenicc(const gs_gstate * pgs, gs_param_string * pval)
{
    static const char *const rfs = "";

     
     
    if (pgs->icc_manager->device_n == NULL) {
        pval->data = (const byte *) rfs;
        pval->persistent = true;
    } else {
        pval->data =
            (const byte *) (pgs->icc_manager->device_n->head->iccprofile->name);
        pval->persistent = false;
    }
    pval->size = strlen((const char *)pval->data);
}
