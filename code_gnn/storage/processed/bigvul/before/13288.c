pdf14_clist_text_begin(gx_device * dev,	gs_gstate	* pgs,
                 const gs_text_params_t * text, gs_font * font,
                 gx_path * path, const gx_device_color * pdcolor,
                 const gx_clip_path * pcpath, gs_memory_t * memory,
                 gs_text_enum_t ** ppenum)
{
    pdf14_clist_device * pdev = (pdf14_clist_device *)dev;
    gs_text_enum_t *penum;
    int code;

    if_debug0m('v', memory, "[v]pdf14_clist_text_begin\n");
     
    code = pdf14_clist_update_params(pdev, pgs, false, NULL);
    if (code < 0)
        return code;
     
    code = gx_forward_text_begin(dev, pgs, text, font, path,
                                pdcolor, pcpath, memory, &penum);
    if (code < 0)
        return code;
    *ppenum = (gs_text_enum_t *)penum;
    return code;
}
