static void opj_get_all_encoding_parameters(const opj_image_t *p_image,
        const opj_cp_t *p_cp,
        OPJ_UINT32 tileno,
        OPJ_INT32 * p_tx0,
        OPJ_INT32 * p_tx1,
        OPJ_INT32 * p_ty0,
        OPJ_INT32 * p_ty1,
        OPJ_UINT32 * p_dx_min,
        OPJ_UINT32 * p_dy_min,
        OPJ_UINT32 * p_max_prec,
        OPJ_UINT32 * p_max_res,
        OPJ_UINT32 ** p_resolutions)
{
     
    OPJ_UINT32 compno, resno;

     
    const opj_tcp_t *tcp = 00;
    const opj_tccp_t * l_tccp = 00;
    const opj_image_comp_t * l_img_comp = 00;

     
    OPJ_UINT32 * lResolutionPtr;

     
    OPJ_UINT32 p, q;

     
    OPJ_UINT32 l_tx0, l_ty0;

     
    assert(p_cp != 00);
    assert(p_image != 00);
    assert(tileno < p_cp->tw * p_cp->th);

     
    tcp = &p_cp->tcps [tileno];
    l_tccp = tcp->tccps;
    l_img_comp = p_image->comps;

     
    p = tileno % p_cp->tw;
    q = tileno / p_cp->tw;

     
    l_tx0 = p_cp->tx0 + p *
            p_cp->tdx;  
    *p_tx0 = (OPJ_INT32)opj_uint_max(l_tx0, p_image->x0);
    *p_tx1 = (OPJ_INT32)opj_uint_min(opj_uint_adds(l_tx0, p_cp->tdx), p_image->x1);
    l_ty0 = p_cp->ty0 + q *
            p_cp->tdy;  
    *p_ty0 = (OPJ_INT32)opj_uint_max(l_ty0, p_image->y0);
    *p_ty1 = (OPJ_INT32)opj_uint_min(opj_uint_adds(l_ty0, p_cp->tdy), p_image->y1);

     
    *p_max_prec = 0;
    *p_max_res = 0;

     
    *p_dx_min = 0x7fffffff;
    *p_dy_min = 0x7fffffff;

    for (compno = 0; compno < p_image->numcomps; ++compno) {
         
        OPJ_UINT32 l_level_no;
        OPJ_INT32 l_rx0, l_ry0, l_rx1, l_ry1;
        OPJ_INT32 l_px0, l_py0, l_px1, py1;
        OPJ_UINT32 l_product;
        OPJ_INT32 l_tcx0, l_tcy0, l_tcx1, l_tcy1;
        OPJ_UINT32 l_pdx, l_pdy, l_pw, l_ph;

        lResolutionPtr = p_resolutions[compno];

        l_tcx0 = opj_int_ceildiv(*p_tx0, (OPJ_INT32)l_img_comp->dx);
        l_tcy0 = opj_int_ceildiv(*p_ty0, (OPJ_INT32)l_img_comp->dy);
        l_tcx1 = opj_int_ceildiv(*p_tx1, (OPJ_INT32)l_img_comp->dx);
        l_tcy1 = opj_int_ceildiv(*p_ty1, (OPJ_INT32)l_img_comp->dy);

        if (l_tccp->numresolutions > *p_max_res) {
            *p_max_res = l_tccp->numresolutions;
        }

         
        l_level_no = l_tccp->numresolutions;
        for (resno = 0; resno < l_tccp->numresolutions; ++resno) {
            OPJ_UINT32 l_dx, l_dy;

            --l_level_no;

             
            l_pdx = l_tccp->prcw[resno];
            l_pdy = l_tccp->prch[resno];
            *lResolutionPtr++ = l_pdx;
            *lResolutionPtr++ = l_pdy;
            l_dx = l_img_comp->dx * (1u << (l_pdx + l_level_no));
            l_dy = l_img_comp->dy * (1u << (l_pdy + l_level_no));
             
            *p_dx_min = (OPJ_UINT32)opj_int_min((OPJ_INT32) * p_dx_min, (OPJ_INT32)l_dx);
            *p_dy_min = (OPJ_UINT32)opj_int_min((OPJ_INT32) * p_dy_min, (OPJ_INT32)l_dy);

             
            l_rx0 = opj_int_ceildivpow2(l_tcx0, (OPJ_INT32)l_level_no);
            l_ry0 = opj_int_ceildivpow2(l_tcy0, (OPJ_INT32)l_level_no);
            l_rx1 = opj_int_ceildivpow2(l_tcx1, (OPJ_INT32)l_level_no);
            l_ry1 = opj_int_ceildivpow2(l_tcy1, (OPJ_INT32)l_level_no);
            l_px0 = opj_int_floordivpow2(l_rx0, (OPJ_INT32)l_pdx) << l_pdx;
            l_py0 = opj_int_floordivpow2(l_ry0, (OPJ_INT32)l_pdy) << l_pdy;
            l_px1 = opj_int_ceildivpow2(l_rx1, (OPJ_INT32)l_pdx) << l_pdx;
            py1 = opj_int_ceildivpow2(l_ry1, (OPJ_INT32)l_pdy) << l_pdy;
            l_pw = (l_rx0 == l_rx1) ? 0 : (OPJ_UINT32)((l_px1 - l_px0) >> l_pdx);
            l_ph = (l_ry0 == l_ry1) ? 0 : (OPJ_UINT32)((py1 - l_py0) >> l_pdy);
            *lResolutionPtr++ = l_pw;
            *lResolutionPtr++ = l_ph;
            l_product = l_pw * l_ph;

             
            if (l_product > *p_max_prec) {
                *p_max_prec = l_product;
            }

        }
        ++l_tccp;
        ++l_img_comp;
    }
}