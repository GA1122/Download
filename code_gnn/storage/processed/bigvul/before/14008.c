void errflush(const gs_memory_t *mem)
{
    if (!mem->gs_lib_ctx->stderr_fn)
         fflush(mem->gs_lib_ctx->fstderr);
      
 }
