alloc_size_is_ok(gs_memory_type_ptr_t stype)
{
    return (stype->ssize > 0 && stype->ssize < 0x200000);
}
