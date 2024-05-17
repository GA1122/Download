void *gs_lib_ctx_get_cms_context( const gs_memory_t *mem )
{
    if (mem == NULL)
        return NULL;
    return mem->gs_lib_ctx->cms_context;
}
