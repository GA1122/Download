void sl_bootparam_write(hwaddr ptr)
{
    cpu_physical_memory_write(ptr, &zaurus_bootparam,
                              sizeof(struct sl_param_info));
}
