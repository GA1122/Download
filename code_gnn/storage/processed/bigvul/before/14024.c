get_minst_from_memory(const gs_memory_t *mem)
{
#ifdef PSI_INCLUDED
    extern gs_main_instance *ps_impl_get_minst( const gs_memory_t *mem );
    return ps_impl_get_minst(mem);
#else
    return (gs_main_instance*)mem->gs_lib_ctx->top_of_system;
#endif
}
