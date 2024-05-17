gsicc_get_spotnames(gcmmhprofile_t profile, gs_memory_t *memory)
{
    int k;
    gsicc_namelist_t *list;
    gsicc_colorname_t *name;
    gsicc_colorname_t **curr_entry;
    int num_colors;
    char *clr_name;

    num_colors = gscms_get_numberclrtnames(profile);
    if (num_colors == 0)
        return(NULL);
     
    list = gsicc_new_namelist(memory);
    if (list == NULL)
        return(NULL);
    curr_entry = &(list->head);
    list->count = num_colors;
    for (k = 0; k < num_colors; k++) {
         
        clr_name = gscms_get_clrtname(profile, k, memory);
        if (clr_name == NULL)
            break;
        name = gsicc_new_colorname(memory);
        if (name == NULL) {
             
            gs_free_object(memory, clr_name, "gsicc_get_spotnames");
            break;
        }
         
        name->name = clr_name;
        name->length = strlen(clr_name);
        *curr_entry = name;
        curr_entry = &(name->next);
    }
    if (k < num_colors) {
         
        gsicc_free_spotnames(list, memory);
        return NULL;
    }
    return list;
}
