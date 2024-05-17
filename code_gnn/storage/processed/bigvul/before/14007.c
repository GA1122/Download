rc_gsicc_manager_free(gs_memory_t * mem, void *ptr_in, client_name_t cname)
{
     
    gsicc_manager_t *icc_manager = (gsicc_manager_t * ) ptr_in;

    gs_free_object(icc_manager->memory, icc_manager, "rc_gsicc_manager_free");
}
