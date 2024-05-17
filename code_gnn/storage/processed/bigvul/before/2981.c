pdf14_transform_color_buffer(gs_gstate *pgs, pdf14_ctx *ctx, gx_device *dev,
    pdf14_buf *src_buf, byte *src_data, cmm_profile_t *src_profile,
    cmm_profile_t *des_profile, int x0, int y0, int width, int height, bool *did_alloc)
{
    gsicc_rendering_param_t rendering_params;
    gsicc_link_t *icc_link;
    gsicc_bufferdesc_t src_buff_desc;
    gsicc_bufferdesc_t des_buff_desc;
    int src_planestride = src_buf->planestride;
    int src_rowstride = src_buf->rowstride;
    int src_n_planes = src_buf->n_planes;
    int src_n_chan = src_buf->n_chan;
    int des_planestride = src_planestride;
    int des_rowstride = src_rowstride;
    int des_n_planes = src_n_planes;
    int des_n_chan = src_n_chan;
    int diff;
    int k, j;
    byte *des_data = NULL;
    pdf14_buf *output = src_buf;
    *did_alloc = false;

     
    if (gsicc_get_hash(src_profile) == gsicc_get_hash(des_profile))
        return src_buf;

     
    rendering_params.black_point_comp = gsBLACKPTCOMP_ON;
    rendering_params.graphics_type_tag = GS_IMAGE_TAG;
    rendering_params.override_icc = false;
    rendering_params.preserve_black = gsBKPRESNOTSPECIFIED;
    rendering_params.rendering_intent = gsPERCEPTUAL;
    rendering_params.cmm = gsCMM_DEFAULT;
    icc_link = gsicc_get_link_profile(pgs, dev, src_profile, des_profile,
        &rendering_params, pgs->memory, false);
    if (icc_link == NULL)
        return NULL;

     
    diff = des_profile->num_comps - src_profile->num_comps;
    if (diff != 0) {
        byte *src_ptr;
        byte *des_ptr;

        *did_alloc = true;
        des_rowstride = (width + 3) & -4;
        des_planestride = height * des_rowstride;
        des_n_planes = src_n_planes + diff;
        des_n_chan = src_n_chan + diff;
        des_data = gs_alloc_bytes(ctx->memory, des_planestride * des_n_planes,
            "pdf14_transform_color_buffer");
        if (des_data == NULL)
            return NULL;

         
        src_ptr = src_data;
        des_ptr = des_data;
        for (j = 0; j < height; j++) {
            for (k = 0; k < (src_n_planes - src_profile->num_comps); k++) {
                memcpy(des_ptr + des_planestride * (k + des_profile->num_comps),
                    src_ptr + src_planestride * (k + src_profile->num_comps),
                    width);
            }
            src_ptr += src_rowstride;
            des_ptr += des_rowstride;
        }
    } else
        des_data = src_data;

     
    gsicc_init_buffer(&src_buff_desc, src_profile->num_comps, 1, false,
        false, true, src_planestride, src_rowstride, height, width);
    gsicc_init_buffer(&des_buff_desc, des_profile->num_comps,
        1, false, false, true, des_planestride,
        des_rowstride, height, width);

     
    (icc_link->procs.map_buffer)(dev, icc_link, &src_buff_desc, &des_buff_desc,
        src_data, des_data);
    gsicc_release_link(icc_link);

    output->planestride = des_planestride;
    output->rowstride = des_rowstride;
    output->n_planes = des_n_planes;
    output->n_chan = des_n_chan;
     
    if (des_data != src_data) {
        gs_free_object(ctx->memory, output->data,
            "pdf14_transform_color_buffer");
        output->data = des_data;
         
        output->rect.p.x = x0;
        output->rect.p.y = y0;
        output->rect.q.x = x0 + width;
        output->rect.q.y = y0 + height;
    }
    return output;
}
