int errwrite(const gs_memory_t *mem, const char *str, int len)
{
    int code;
    gs_lib_ctx_t *ctx;
    if (len == 0)
        return 0;
    if (mem == NULL) {
#ifdef GS_THREADSAFE
        return 0;
#else
        mem = mem_err_print;
        if (mem == NULL)
            return 0;
#endif
    }
    ctx = mem->gs_lib_ctx;
    if (ctx == NULL)
      return 0;
    if (ctx->stderr_fn)
        return (*ctx->stderr_fn)(ctx->caller_handle, str, len);

    code = fwrite(str, 1, len, ctx->fstderr);
    fflush(ctx->fstderr);
    return code;
}
