gsicc_free_spotnames(gsicc_namelist_t *spotnames, gs_memory_t * mem)
{
    int k;
    gsicc_colorname_t *curr_name, *next_name;

    curr_name = spotnames->head;
    for (k = 0; k < spotnames->count; k++) {
        next_name = curr_name->next;
         
        gs_free_object(mem, curr_name->name, "gsicc_free_spotnames");
         
        gs_free_object(mem, curr_name, "gsicc_free_spotnames");
        curr_name = next_name;
    }
    if (spotnames->color_map != NULL) {
        gs_free_object(mem, spotnames->color_map, "gsicc_free_spotnames");
    }
    if (spotnames->name_str != NULL) {
        gs_free_object(mem, spotnames->name_str, "gsicc_free_spotnames");
    }
}
