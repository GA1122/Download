gs_pdf14_clist_device_push(gs_memory_t *mem, gs_gstate *pgs, gx_device **pcdev,
                           gx_device *dev, const gs_pdf14trans_t *pdf14pct)
{
    int code;
    pdf14_clist_device *p14dev;
    gx_device_clist_writer * const cdev = &((gx_device_clist *)dev)->writer;

    code = pdf14_create_clist_device(mem, pgs, pcdev, dev, pdf14pct);
     
    p14dev = (pdf14_clist_device *)(*pcdev);
    p14dev->saved_target_color_info = dev->color_info;
    dev->color_info = (*pcdev)->color_info;
     
    dev->color_info.anti_alias = p14dev->saved_target_color_info.anti_alias;
    p14dev->color_info.anti_alias = dev->color_info.anti_alias;

     
    cdev->clist_color_info.depth = p14dev->color_info.depth;
    cdev->clist_color_info.polarity = p14dev->color_info.polarity;
    cdev->clist_color_info.num_components = p14dev->color_info.num_components;
    cdev->clist_color_info.max_color = p14dev->color_info.max_color;
    cdev->clist_color_info.max_gray = p14dev->color_info.max_gray;

    p14dev->saved_target_encode_color = dev->procs.encode_color;
    p14dev->saved_target_decode_color = dev->procs.decode_color;
    dev->procs.encode_color = p14dev->procs.encode_color =
                              p14dev->my_encode_color;
    dev->procs.decode_color = p14dev->procs.decode_color =
                              p14dev->my_decode_color;
    p14dev->saved_target_get_color_mapping_procs =
                              dev->procs.get_color_mapping_procs;
    p14dev->saved_target_get_color_comp_index =
                              dev->procs.get_color_comp_index;
    dev->procs.get_color_mapping_procs =
        p14dev->procs.get_color_mapping_procs =
        p14dev->my_get_color_mapping_procs;
    dev->procs.get_color_comp_index =
        p14dev->procs.get_color_comp_index =
        p14dev->my_get_color_comp_index;
    p14dev->save_get_cmap_procs = pgs->get_cmap_procs;
    pgs->get_cmap_procs = pdf14_get_cmap_procs;
    gx_set_cmap_procs(pgs, dev);
    return code;
}
