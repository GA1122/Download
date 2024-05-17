gs_lib_ctx_set_icc_directory(const gs_memory_t *mem_gc, const char* pname,
                        int dir_namelen)
{
    char *result;
    gs_lib_ctx_t *p_ctx = mem_gc->gs_lib_ctx;
    gs_memory_t *p_ctx_mem = p_ctx->memory;

     
    if (p_ctx->profiledir != NULL && strcmp(pname,DEFAULT_DIR_ICC) == 0) {
        return;
    }
    if (p_ctx->profiledir != NULL && p_ctx->profiledir_len > 0) {
        if (strncmp(pname, p_ctx->profiledir, p_ctx->profiledir_len) == 0) {
            return;
        }
        gs_free_object(p_ctx_mem, p_ctx->profiledir,
                       "gs_lib_ctx_set_icc_directory");
    }
     
    result = (char*) gs_alloc_bytes(p_ctx_mem, dir_namelen+1,
                                     "gs_lib_ctx_set_icc_directory");
    if (result != NULL) {
        strcpy(result, pname);
        p_ctx->profiledir = result;
        p_ctx->profiledir_len = dir_namelen;
    }
}
