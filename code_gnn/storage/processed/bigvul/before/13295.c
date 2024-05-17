pdf14_cmap_separation_direct(frac all, gx_device_color * pdc, const gs_gstate * pgs,
                 gx_device * dev, gs_color_select_t select)
{
    int i, ncomps = dev->color_info.num_components;
    int num_spots = pdf14_get_num_spots(dev);
    bool additive = dev->color_info.polarity == GX_CINFO_POLARITY_ADDITIVE;
    frac comp_value = all;
    frac cm_comps[GX_DEVICE_COLOR_MAX_COMPONENTS];
    gx_color_value cv[GX_DEVICE_COLOR_MAX_COMPONENTS];
    gx_color_index color;

    if (pgs->color_component_map.sep_type == SEP_ALL) {
         
        if (additive)
            comp_value = frac_1 - comp_value;
         
        i = pgs->color_component_map.num_colorants - 1;
        for (; i >= 0; i--)
            cm_comps[i] = comp_value;
    } else {
         
        map_components_to_colorants(&comp_value, &(pgs->color_component_map), cm_comps);
    }
     
    if (additive) {
        for (i = 0; i < ncomps; i++)
            cv[i] = frac2cv(gx_map_color_frac(pgs, cm_comps[i], effective_transfer[i]));
         
        if (pgs->color_component_map.sep_type != SEP_ALL)
            for (i = 0; i < ncomps - num_spots; i++)
                cv[i] = gx_max_color_value;
    } else
        for (i = 0; i < ncomps; i++)
            cv[i] = frac2cv(frac_1 - gx_map_color_frac(pgs, (frac)(frac_1 - cm_comps[i]), effective_transfer[i]));


     
    if (dev_proc(dev, dev_spec_op)(dev, gxdso_supports_devn, NULL, 0)) {
        for (i = 0; i < ncomps; i++)
            pdc->colors.devn.values[i] = cv[i];
        pdc->type = gx_dc_type_devn;
    } else {
         
        color = dev_proc(dev, encode_color)(dev, cv);
         
        if (color != gx_no_color_index)
            color_set_pure(pdc, color);
    }
}
