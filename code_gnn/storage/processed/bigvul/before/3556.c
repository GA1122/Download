gs_malloc_wrap(gs_memory_t **wrapped, gs_malloc_memory_t *contents)
{
#  ifdef USE_RETRY_MEMORY_WRAPPER
     
    {
        gs_memory_retrying_t *rmem;
        rmem = (gs_memory_retrying_t *)
            gs_alloc_bytes_immovable((gs_memory_t *)lmem,
                                     sizeof(gs_memory_retrying_t),
                                     "gs_malloc_wrap(retrying)");
        if (rmem == 0) {
            gs_memory_locked_release(lmem);
            gs_free_object(cmem, lmem, "gs_malloc_wrap(locked)");
            return_error(gs_error_VMerror);
        }
        code = gs_memory_retrying_init(rmem, (gs_memory_t *)lmem);
        if (code < 0) {
            gs_free_object((gs_memory_t *)lmem, rmem, "gs_malloc_wrap(retrying)");
            gs_memory_locked_release(lmem);
            gs_free_object(cmem, lmem, "gs_malloc_wrap(locked)");
            return code;
        }

        *wrapped = (gs_memory_t *)rmem;
    }
#  endif  
    return 0;
}
