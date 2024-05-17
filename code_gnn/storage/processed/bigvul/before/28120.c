av_cold void ff_dsputil_init(DSPContext* c, AVCodecContext *avctx)
{
    ff_check_alignment();

#if CONFIG_ENCODERS
    if (avctx->bits_per_raw_sample == 10) {
        c->fdct    = ff_jpeg_fdct_islow_10;
        c->fdct248 = ff_fdct248_islow_10;
    } else {
        if(avctx->dct_algo==FF_DCT_FASTINT) {
            c->fdct    = ff_fdct_ifast;
            c->fdct248 = ff_fdct_ifast248;
        }
        else if(avctx->dct_algo==FF_DCT_FAAN) {
            c->fdct    = ff_faandct;
            c->fdct248 = ff_faandct248;
        }
        else {
            c->fdct    = ff_jpeg_fdct_islow_8;  
            c->fdct248 = ff_fdct248_islow_8;
        }
    }
#endif  

    if(avctx->lowres==1){
        c->idct_put= ff_jref_idct4_put;
        c->idct_add= ff_jref_idct4_add;
        c->idct    = ff_j_rev_dct4;
        c->idct_permutation_type= FF_NO_IDCT_PERM;
    }else if(avctx->lowres==2){
        c->idct_put= ff_jref_idct2_put;
        c->idct_add= ff_jref_idct2_add;
        c->idct    = ff_j_rev_dct2;
        c->idct_permutation_type= FF_NO_IDCT_PERM;
    }else if(avctx->lowres==3){
        c->idct_put= ff_jref_idct1_put;
        c->idct_add= ff_jref_idct1_add;
        c->idct    = ff_j_rev_dct1;
        c->idct_permutation_type= FF_NO_IDCT_PERM;
    }else{
        if (avctx->bits_per_raw_sample == 10) {
            c->idct_put              = ff_simple_idct_put_10;
            c->idct_add              = ff_simple_idct_add_10;
            c->idct                  = ff_simple_idct_10;
            c->idct_permutation_type = FF_NO_IDCT_PERM;
        } else {
        if(avctx->idct_algo==FF_IDCT_INT){
            c->idct_put= jref_idct_put;
            c->idct_add= jref_idct_add;
            c->idct    = ff_j_rev_dct;
            c->idct_permutation_type= FF_LIBMPEG2_IDCT_PERM;
        }else if(avctx->idct_algo==FF_IDCT_FAAN){
            c->idct_put= ff_faanidct_put;
            c->idct_add= ff_faanidct_add;
            c->idct    = ff_faanidct;
            c->idct_permutation_type= FF_NO_IDCT_PERM;
        }else{  
            c->idct_put = ff_simple_idct_put_8;
            c->idct_add = ff_simple_idct_add_8;
            c->idct     = ff_simple_idct_8;
            c->idct_permutation_type= FF_NO_IDCT_PERM;
        }
        }
    }

    c->diff_pixels = diff_pixels_c;
    c->put_pixels_clamped = put_pixels_clamped_c;
    c->put_signed_pixels_clamped = put_signed_pixels_clamped_c;
    c->add_pixels_clamped = add_pixels_clamped_c;
    c->sum_abs_dctelem = sum_abs_dctelem_c;
    c->gmc1 = gmc1_c;
    c->gmc = ff_gmc_c;
    c->pix_sum = pix_sum_c;
    c->pix_norm1 = pix_norm1_c;

    c->fill_block_tab[0] = fill_block16_c;
    c->fill_block_tab[1] = fill_block8_c;

     
    c->pix_abs[0][0] = pix_abs16_c;
    c->pix_abs[0][1] = pix_abs16_x2_c;
    c->pix_abs[0][2] = pix_abs16_y2_c;
    c->pix_abs[0][3] = pix_abs16_xy2_c;
    c->pix_abs[1][0] = pix_abs8_c;
    c->pix_abs[1][1] = pix_abs8_x2_c;
    c->pix_abs[1][2] = pix_abs8_y2_c;
    c->pix_abs[1][3] = pix_abs8_xy2_c;

    c->put_tpel_pixels_tab[ 0] = put_tpel_pixels_mc00_c;
    c->put_tpel_pixels_tab[ 1] = put_tpel_pixels_mc10_c;
    c->put_tpel_pixels_tab[ 2] = put_tpel_pixels_mc20_c;
    c->put_tpel_pixels_tab[ 4] = put_tpel_pixels_mc01_c;
    c->put_tpel_pixels_tab[ 5] = put_tpel_pixels_mc11_c;
    c->put_tpel_pixels_tab[ 6] = put_tpel_pixels_mc21_c;
    c->put_tpel_pixels_tab[ 8] = put_tpel_pixels_mc02_c;
    c->put_tpel_pixels_tab[ 9] = put_tpel_pixels_mc12_c;
    c->put_tpel_pixels_tab[10] = put_tpel_pixels_mc22_c;

    c->avg_tpel_pixels_tab[ 0] = avg_tpel_pixels_mc00_c;
    c->avg_tpel_pixels_tab[ 1] = avg_tpel_pixels_mc10_c;
    c->avg_tpel_pixels_tab[ 2] = avg_tpel_pixels_mc20_c;
    c->avg_tpel_pixels_tab[ 4] = avg_tpel_pixels_mc01_c;
    c->avg_tpel_pixels_tab[ 5] = avg_tpel_pixels_mc11_c;
    c->avg_tpel_pixels_tab[ 6] = avg_tpel_pixels_mc21_c;
    c->avg_tpel_pixels_tab[ 8] = avg_tpel_pixels_mc02_c;
    c->avg_tpel_pixels_tab[ 9] = avg_tpel_pixels_mc12_c;
    c->avg_tpel_pixels_tab[10] = avg_tpel_pixels_mc22_c;

#define dspfunc(PFX, IDX, NUM) \
    c->PFX ## _pixels_tab[IDX][ 0] = PFX ## NUM ## _mc00_c; \
    c->PFX ## _pixels_tab[IDX][ 1] = PFX ## NUM ## _mc10_c; \
    c->PFX ## _pixels_tab[IDX][ 2] = PFX ## NUM ## _mc20_c; \
    c->PFX ## _pixels_tab[IDX][ 3] = PFX ## NUM ## _mc30_c; \
    c->PFX ## _pixels_tab[IDX][ 4] = PFX ## NUM ## _mc01_c; \
    c->PFX ## _pixels_tab[IDX][ 5] = PFX ## NUM ## _mc11_c; \
    c->PFX ## _pixels_tab[IDX][ 6] = PFX ## NUM ## _mc21_c; \
    c->PFX ## _pixels_tab[IDX][ 7] = PFX ## NUM ## _mc31_c; \
    c->PFX ## _pixels_tab[IDX][ 8] = PFX ## NUM ## _mc02_c; \
    c->PFX ## _pixels_tab[IDX][ 9] = PFX ## NUM ## _mc12_c; \
    c->PFX ## _pixels_tab[IDX][10] = PFX ## NUM ## _mc22_c; \
    c->PFX ## _pixels_tab[IDX][11] = PFX ## NUM ## _mc32_c; \
    c->PFX ## _pixels_tab[IDX][12] = PFX ## NUM ## _mc03_c; \
    c->PFX ## _pixels_tab[IDX][13] = PFX ## NUM ## _mc13_c; \
    c->PFX ## _pixels_tab[IDX][14] = PFX ## NUM ## _mc23_c; \
    c->PFX ## _pixels_tab[IDX][15] = PFX ## NUM ## _mc33_c

    dspfunc(put_qpel, 0, 16);
    dspfunc(put_no_rnd_qpel, 0, 16);

    dspfunc(avg_qpel, 0, 16);
     

    dspfunc(put_qpel, 1, 8);
    dspfunc(put_no_rnd_qpel, 1, 8);

    dspfunc(avg_qpel, 1, 8);
     

#undef dspfunc

    c->put_mspel_pixels_tab[0]= ff_put_pixels8x8_c;
    c->put_mspel_pixels_tab[1]= put_mspel8_mc10_c;
    c->put_mspel_pixels_tab[2]= put_mspel8_mc20_c;
    c->put_mspel_pixels_tab[3]= put_mspel8_mc30_c;
    c->put_mspel_pixels_tab[4]= put_mspel8_mc02_c;
    c->put_mspel_pixels_tab[5]= put_mspel8_mc12_c;
    c->put_mspel_pixels_tab[6]= put_mspel8_mc22_c;
    c->put_mspel_pixels_tab[7]= put_mspel8_mc32_c;

#define SET_CMP_FUNC(name) \
    c->name[0]= name ## 16_c;\
    c->name[1]= name ## 8x8_c;

    SET_CMP_FUNC(hadamard8_diff)
    c->hadamard8_diff[4]= hadamard8_intra16_c;
    c->hadamard8_diff[5]= hadamard8_intra8x8_c;
    SET_CMP_FUNC(dct_sad)
    SET_CMP_FUNC(dct_max)
#if CONFIG_GPL
    SET_CMP_FUNC(dct264_sad)
#endif
    c->sad[0]= pix_abs16_c;
    c->sad[1]= pix_abs8_c;
    c->sse[0]= sse16_c;
    c->sse[1]= sse8_c;
    c->sse[2]= sse4_c;
    SET_CMP_FUNC(quant_psnr)
    SET_CMP_FUNC(rd)
    SET_CMP_FUNC(bit)
    c->vsad[0]= vsad16_c;
    c->vsad[4]= vsad_intra16_c;
    c->vsad[5]= vsad_intra8_c;
    c->vsse[0]= vsse16_c;
    c->vsse[4]= vsse_intra16_c;
    c->vsse[5]= vsse_intra8_c;
    c->nsse[0]= nsse16_c;
    c->nsse[1]= nsse8_c;
#if CONFIG_SNOW_DECODER || CONFIG_SNOW_ENCODER
    ff_dsputil_init_dwt(c);
#endif

    c->ssd_int8_vs_int16 = ssd_int8_vs_int16_c;

    c->add_bytes= add_bytes_c;
    c->diff_bytes= diff_bytes_c;
    c->add_hfyu_median_prediction= add_hfyu_median_prediction_c;
    c->sub_hfyu_median_prediction= sub_hfyu_median_prediction_c;
    c->add_hfyu_left_prediction  = add_hfyu_left_prediction_c;
    c->add_hfyu_left_prediction_bgr32 = add_hfyu_left_prediction_bgr32_c;
    c->bswap_buf= bswap_buf;
    c->bswap16_buf = bswap16_buf;

    if (CONFIG_H263_DECODER || CONFIG_H263_ENCODER) {
        c->h263_h_loop_filter= h263_h_loop_filter_c;
        c->h263_v_loop_filter= h263_v_loop_filter_c;
    }

    c->try_8x8basis= try_8x8basis_c;
    c->add_8x8basis= add_8x8basis_c;

    c->vector_clipf = vector_clipf_c;
    c->scalarproduct_int16 = scalarproduct_int16_c;
    c->scalarproduct_and_madd_int16 = scalarproduct_and_madd_int16_c;
    c->apply_window_int16 = apply_window_int16_c;
    c->vector_clip_int32 = vector_clip_int32_c;

    c->shrink[0]= av_image_copy_plane;
    c->shrink[1]= ff_shrink22;
    c->shrink[2]= ff_shrink44;
    c->shrink[3]= ff_shrink88;

    c->add_pixels8 = add_pixels8_c;

#undef FUNC
#undef FUNCC
#define FUNC(f, depth) f ## _ ## depth
#define FUNCC(f, depth) f ## _ ## depth ## _c

    c->draw_edges                    = FUNCC(draw_edges, 8);
    c->clear_block                   = FUNCC(clear_block, 8);
    c->clear_blocks                  = FUNCC(clear_blocks, 8);

#define BIT_DEPTH_FUNCS(depth) \
    c->get_pixels                    = FUNCC(get_pixels,   depth);

    switch (avctx->bits_per_raw_sample) {
    case 9:
    case 10:
    case 12:
    case 14:
        BIT_DEPTH_FUNCS(16);
        break;
    default:
        if(avctx->bits_per_raw_sample<=8 || avctx->codec_type != AVMEDIA_TYPE_VIDEO) {
            BIT_DEPTH_FUNCS(8);
        }
        break;
    }


    if (ARCH_ALPHA)
        ff_dsputil_init_alpha(c, avctx);
    if (ARCH_ARM)
        ff_dsputil_init_arm(c, avctx);
    if (ARCH_BFIN)
        ff_dsputil_init_bfin(c, avctx);
    if (ARCH_PPC)
        ff_dsputil_init_ppc(c, avctx);
    if (ARCH_SH4)
        ff_dsputil_init_sh4(c, avctx);
    if (HAVE_VIS)
        ff_dsputil_init_vis(c, avctx);
    if (ARCH_X86)
        ff_dsputil_init_x86(c, avctx);

    ff_init_scantable_permutation(c->idct_permutation,
                                  c->idct_permutation_type);
}