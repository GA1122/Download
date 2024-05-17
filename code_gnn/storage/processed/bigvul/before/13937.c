gs_currenticcdirectory(const gs_gstate * pgs, gs_param_string * pval)
{
    static const char *const rfs = DEFAULT_DIR_ICC;    
    const gs_lib_ctx_t *lib_ctx = pgs->memory->gs_lib_ctx;

    if (lib_ctx->profiledir == NULL) {
        pval->data = (const byte *)rfs;
        pval->size = strlen(rfs);
        pval->persistent = true;
    } else {
        pval->data = (const byte *)(lib_ctx->profiledir);
        pval->size = lib_ctx->profiledir_len;
        pval->persistent = false;
    }
}
