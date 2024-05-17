void gs_lib_ctx_fin(gs_memory_t *mem)
{
    gs_lib_ctx_t *ctx;
    gs_memory_t *ctx_mem;

    if (!mem || !mem->gs_lib_ctx)
        return;
    
    ctx = mem->gs_lib_ctx;
    ctx_mem = ctx->memory;

    sjpxd_destroy(mem);
    gscms_destroy(ctx_mem);
    gs_free_object(ctx_mem, ctx->profiledir,
        "gs_lib_ctx_fin");
        
    gs_free_object(ctx_mem, ctx->default_device_list,
                "gs_lib_ctx_fin");

#ifndef GS_THREADSAFE
    mem_err_print = NULL;
#endif
    remove_ctx_pointers(ctx_mem);
    gs_free_object(ctx_mem, ctx, "gs_lib_ctx_init");
}
