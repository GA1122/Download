gs_heap_free_string(gs_memory_t * mem, byte * data, uint nbytes,
                    client_name_t cname)
{
     
    gs_heap_free_object(mem, data, cname);
}
