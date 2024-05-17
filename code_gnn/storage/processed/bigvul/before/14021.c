void outflush(const gs_memory_t *mem)
{
    if (mem->gs_lib_ctx->stdout_is_redirected) {
        if (mem->gs_lib_ctx->stdout_to_stderr) {
            if (!mem->gs_lib_ctx->stderr_fn)
                fflush(mem->gs_lib_ctx->fstderr);
        }
        else
            fflush(mem->gs_lib_ctx->fstdout2);
    }
    else if (!mem->gs_lib_ctx->stdout_fn)
        fflush(mem->gs_lib_ctx->fstdout);
}
