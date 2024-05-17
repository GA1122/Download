pdf14_spot_get_color_comp_index(gx_device *dev, const char *pname,
    int name_size, int component_type, int num_process_colors)
{
    pdf14_device *pdev = (pdf14_device *)dev;
    gx_device *tdev = pdev->target;
    gs_devn_params *pdevn_params = &pdev->devn_params;
    gs_separations *pseparations = &pdevn_params->separations;
    int comp_index;
    dev_proc_get_color_comp_index(*target_get_color_comp_index);
    int offset = 4 - num_process_colors;


    while (tdev->child) {
        tdev = tdev->child;
    }

     
    if (num_process_colors < 4) {
        int k;
        for (k = 0; k < 4; k++) {
            if (strncmp(pname, pdev->devn_params.std_colorant_names[k], name_size) == 0)
                return -1;
        }
    }

    target_get_color_comp_index = dev_proc(tdev, get_color_comp_index);

     
    if (target_get_color_comp_index == pdf14_cmykspot_get_color_comp_index)
        target_get_color_comp_index =
        ((pdf14_clist_device *)pdev)->saved_target_get_color_comp_index;
     
    if (component_type == NO_COMP_NAME_TYPE)
        return  (*target_get_color_comp_index)(tdev, pname, name_size, component_type);
     
    comp_index = check_pcm_and_separation_names(dev, pdevn_params, pname,
        name_size, component_type);
     
    if (comp_index >= 0)
        return comp_index - offset;
     
    comp_index = (*target_get_color_comp_index)(tdev, pname, name_size, component_type);
     
    if (comp_index < 0 || comp_index == GX_DEVICE_COLOR_MAX_COMPONENTS)
        return comp_index - offset;

     
    if (pseparations->num_separations < GX_DEVICE_COLOR_MAX_COMPONENTS - 1) {
        int sep_num = pseparations->num_separations++;
        int color_component_number;
        byte * sep_name;

        sep_name = gs_alloc_bytes(dev->memory->stable_memory,
            name_size, "pdf14_spot_get_color_comp_index");
        memcpy(sep_name, pname, name_size);
        pseparations->names[sep_num].size = name_size;
        pseparations->names[sep_num].data = sep_name;
        color_component_number = sep_num + num_process_colors;
        if (color_component_number >= dev->color_info.num_components)
            color_component_number = GX_DEVICE_COLOR_MAX_COMPONENTS;
        else
            pdevn_params->separation_order_map[color_component_number] =
            color_component_number;
        return color_component_number;
    }

    return GX_DEVICE_COLOR_MAX_COMPONENTS;
}
