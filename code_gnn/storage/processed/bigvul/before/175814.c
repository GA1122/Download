static void decode_macroblock(VP8D_COMP *pbi, MACROBLOCKD *xd,
 unsigned int mb_idx)
{
    MB_PREDICTION_MODE mode;
 int i;
#if CONFIG_ERROR_CONCEALMENT
 int corruption_detected = 0;
#else
 (void)mb_idx;
#endif

 if (xd->mode_info_context->mbmi.mb_skip_coeff)
 {
        vp8_reset_mb_tokens_context(xd);
 }
 else if (!vp8dx_bool_error(xd->current_bc))
 {
 int eobtotal;
        eobtotal = vp8_decode_mb_tokens(pbi, xd);

  
        xd->mode_info_context->mbmi.mb_skip_coeff = (eobtotal==0);
 }

    mode = xd->mode_info_context->mbmi.mode;

 if (xd->segmentation_enabled)
        vp8_mb_init_dequantizer(pbi, xd);


#if CONFIG_ERROR_CONCEALMENT

 if(pbi->ec_active)
 {
 int throw_residual;
  
        throw_residual = (!pbi->independent_partitions &&
                          pbi->frame_corrupt_residual);
        throw_residual = (throw_residual || vp8dx_bool_error(xd->current_bc));

 if ((mb_idx >= pbi->mvs_corrupt_from_mb || throw_residual))
 {
  
            pbi->frame_corrupt_residual = 1;
            memset(xd->qcoeff, 0, sizeof(xd->qcoeff));
            vp8_conceal_corrupt_mb(xd);


            corruption_detected = 1;

  
            memset(xd->eobs, 0, 25);
 }
 }
#endif

  
 if (xd->mode_info_context->mbmi.ref_frame == INTRA_FRAME)
 {
        vp8_build_intra_predictors_mbuv_s(xd,
                                          xd->recon_above[1],
                                          xd->recon_above[2],
                                          xd->recon_left[1],
                                          xd->recon_left[2],
                                          xd->recon_left_stride[1],
                                          xd->dst.u_buffer, xd->dst.v_buffer,
                                          xd->dst.uv_stride);

 if (mode != B_PRED)
 {
            vp8_build_intra_predictors_mby_s(xd,
                                                 xd->recon_above[0],
                                                 xd->recon_left[0],
                                                 xd->recon_left_stride[0],
                                                 xd->dst.y_buffer,
                                                 xd->dst.y_stride);
 }
 else
 {
 short *DQC = xd->dequant_y1;
 int dst_stride = xd->dst.y_stride;

  
 if(xd->mode_info_context->mbmi.mb_skip_coeff)
                memset(xd->eobs, 0, 25);

            intra_prediction_down_copy(xd, xd->recon_above[0] + 16);

 for (i = 0; i < 16; i++)
 {
                BLOCKD *b = &xd->block[i];
 unsigned char *dst = xd->dst.y_buffer + b->offset;
                B_PREDICTION_MODE b_mode =
                    xd->mode_info_context->bmi[i].as_mode;
 unsigned char *Above = dst - dst_stride;
 unsigned char *yleft = dst - 1;
 int left_stride = dst_stride;
 unsigned char top_left = Above[-1];

                vp8_intra4x4_predict(Above, yleft, left_stride, b_mode,
                                     dst, dst_stride, top_left);

 if (xd->eobs[i])
 {
 if (xd->eobs[i] > 1)
 {
                    vp8_dequant_idct_add(b->qcoeff, DQC, dst, dst_stride);
 }
 else
 {
                        vp8_dc_only_idct_add
 (b->qcoeff[0] * DQC[0],
                                dst, dst_stride,
                                dst, dst_stride);
                        memset(b->qcoeff, 0, 2 * sizeof(b->qcoeff[0]));
 }
 }
 }
 }
 }
 else
 {
        vp8_build_inter_predictors_mb(xd);
 }


#if CONFIG_ERROR_CONCEALMENT
 if (corruption_detected)
 {
 return;
 }
#endif

 if(!xd->mode_info_context->mbmi.mb_skip_coeff)
 {
  
 if (mode != B_PRED)
 {
 short *DQC = xd->dequant_y1;

 if (mode != SPLITMV)
 {
                BLOCKD *b = &xd->block[24];

  
 if (xd->eobs[24] > 1)
 {
                    vp8_dequantize_b(b, xd->dequant_y2);

                    vp8_short_inv_walsh4x4(&b->dqcoeff[0],
                        xd->qcoeff);
                    memset(b->qcoeff, 0, 16 * sizeof(b->qcoeff[0]));
 }
 else
 {
                    b->dqcoeff[0] = b->qcoeff[0] * xd->dequant_y2[0];
                    vp8_short_inv_walsh4x4_1(&b->dqcoeff[0],
                        xd->qcoeff);
                    memset(b->qcoeff, 0, 2 * sizeof(b->qcoeff[0]));
 }

  
                DQC = xd->dequant_y1_dc;
 }

            vp8_dequant_idct_add_y_block
 (xd->qcoeff, DQC,
                             xd->dst.y_buffer,
                             xd->dst.y_stride, xd->eobs);
 }

        vp8_dequant_idct_add_uv_block
 (xd->qcoeff+16*16, xd->dequant_uv,
                         xd->dst.u_buffer, xd->dst.v_buffer,
                         xd->dst.uv_stride, xd->eobs+16);
 }
}
