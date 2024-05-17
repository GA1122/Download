static void mt_decode_mb_rows(VP8D_COMP *pbi, MACROBLOCKD *xd, int start_mb_row)
{
 volatile const int *last_row_current_mb_col;
 volatile int *current_mb_col;
 int mb_row;
    VP8_COMMON *pc = &pbi->common;
 const int nsync = pbi->sync_range;
 const int first_row_no_sync_above = pc->mb_cols + nsync;
 int num_part = 1 << pbi->common.multi_token_partition;
 int last_mb_row = start_mb_row;

    YV12_BUFFER_CONFIG *yv12_fb_new = pbi->dec_fb_ref[INTRA_FRAME];
    YV12_BUFFER_CONFIG *yv12_fb_lst = pbi->dec_fb_ref[LAST_FRAME];

 int recon_y_stride = yv12_fb_new->y_stride;
 int recon_uv_stride = yv12_fb_new->uv_stride;

 unsigned char *ref_buffer[MAX_REF_FRAMES][3];
 unsigned char *dst_buffer[3];
 int i;
 int ref_fb_corrupted[MAX_REF_FRAMES];

    ref_fb_corrupted[INTRA_FRAME] = 0;

 for(i = 1; i < MAX_REF_FRAMES; i++)
 {
        YV12_BUFFER_CONFIG *this_fb = pbi->dec_fb_ref[i];

        ref_buffer[i][0] = this_fb->y_buffer;
        ref_buffer[i][1] = this_fb->u_buffer;
        ref_buffer[i][2] = this_fb->v_buffer;

        ref_fb_corrupted[i] = this_fb->corrupted;
 }

    dst_buffer[0] = yv12_fb_new->y_buffer;
    dst_buffer[1] = yv12_fb_new->u_buffer;
    dst_buffer[2] = yv12_fb_new->v_buffer;

    xd->up_available = (start_mb_row != 0);

 for (mb_row = start_mb_row; mb_row < pc->mb_rows; mb_row += (pbi->decoding_thread_count + 1))
 {
 int recon_yoffset, recon_uvoffset;
 int mb_col;
 int filter_level;
       loop_filter_info_n *lfi_n = &pc->lf_info;

  
       last_mb_row = mb_row;
  
       xd->current_bc = &pbi->mbc[mb_row%num_part];

 if (mb_row > 0)
           last_row_current_mb_col = &pbi->mt_current_mb_col[mb_row -1];
 else
           last_row_current_mb_col = &first_row_no_sync_above;

       current_mb_col = &pbi->mt_current_mb_col[mb_row];

       recon_yoffset = mb_row * recon_y_stride * 16;
       recon_uvoffset = mb_row * recon_uv_stride * 8;

  
       xd->above_context = pc->above_context;
       memset(xd->left_context, 0, sizeof(ENTROPY_CONTEXT_PLANES));

       xd->left_available = 0;

       xd->mb_to_top_edge = -((mb_row * 16)) << 3;
       xd->mb_to_bottom_edge = ((pc->mb_rows - 1 - mb_row) * 16) << 3;

 if (pbi->common.filter_level)
 {
          xd->recon_above[0] = pbi->mt_yabove_row[mb_row] + 0*16 +32;
          xd->recon_above[1] = pbi->mt_uabove_row[mb_row] + 0*8 +16;
          xd->recon_above[2] = pbi->mt_vabove_row[mb_row] + 0*8 +16;

          xd->recon_left[0] = pbi->mt_yleft_col[mb_row];
          xd->recon_left[1] = pbi->mt_uleft_col[mb_row];
          xd->recon_left[2] = pbi->mt_vleft_col[mb_row];

  
          xd->recon_left_stride[0] = 1;
          xd->recon_left_stride[1] = 1;
 }
 else
 {
          xd->recon_above[0] = dst_buffer[0] + recon_yoffset;
          xd->recon_above[1] = dst_buffer[1] + recon_uvoffset;
          xd->recon_above[2] = dst_buffer[2] + recon_uvoffset;

          xd->recon_left[0] = xd->recon_above[0] - 1;
          xd->recon_left[1] = xd->recon_above[1] - 1;
          xd->recon_left[2] = xd->recon_above[2] - 1;

          xd->recon_above[0] -= xd->dst.y_stride;
          xd->recon_above[1] -= xd->dst.uv_stride;
          xd->recon_above[2] -= xd->dst.uv_stride;

  
          xd->recon_left_stride[0] = xd->dst.y_stride;
          xd->recon_left_stride[1] = xd->dst.uv_stride;

          setup_intra_recon_left(xd->recon_left[0], xd->recon_left[1],
                                 xd->recon_left[2], xd->dst.y_stride,
                                 xd->dst.uv_stride);
 }

 for (mb_col = 0; mb_col < pc->mb_cols; mb_col++)
 {
 *current_mb_col = mb_col - 1;

 if ((mb_col & (nsync - 1)) == 0)
 {
 while (mb_col > (*last_row_current_mb_col - nsync))
 {
                   x86_pause_hint();
                   thread_sleep(0);
 }
 }

  
           xd->mb_to_left_edge = -((mb_col * 16) << 3);
           xd->mb_to_right_edge = ((pc->mb_cols - 1 - mb_col) * 16) << 3;

 #if CONFIG_ERROR_CONCEALMENT
 {
 int corrupt_residual =
 (!pbi->independent_partitions &&
                           pbi->frame_corrupt_residual) ||
                           vp8dx_bool_error(xd->current_bc);
 if (pbi->ec_active &&
 (xd->mode_info_context->mbmi.ref_frame ==
                                                    INTRA_FRAME) &&
                   corrupt_residual)
 {
  
                   vp8_interpolate_motion(xd,
                                          mb_row, mb_col,
                                          pc->mb_rows, pc->mb_cols,
                                          pc->mode_info_stride);
 }
 }
 #endif


           xd->dst.y_buffer = dst_buffer[0] + recon_yoffset;
           xd->dst.u_buffer = dst_buffer[1] + recon_uvoffset;
           xd->dst.v_buffer = dst_buffer[2] + recon_uvoffset;

           xd->pre.y_buffer = ref_buffer[xd->mode_info_context->mbmi.ref_frame][0] + recon_yoffset;
           xd->pre.u_buffer = ref_buffer[xd->mode_info_context->mbmi.ref_frame][1] + recon_uvoffset;
           xd->pre.v_buffer = ref_buffer[xd->mode_info_context->mbmi.ref_frame][2] + recon_uvoffset;

  
           xd->corrupted |= ref_fb_corrupted[xd->mode_info_context->mbmi.ref_frame];

           mt_decode_macroblock(pbi, xd, 0);

           xd->left_available = 1;

  
           xd->corrupted |= vp8dx_bool_error(xd->current_bc);

           xd->recon_above[0] += 16;
           xd->recon_above[1] += 8;
           xd->recon_above[2] += 8;

 if (!pbi->common.filter_level)
 {
              xd->recon_left[0] += 16;
              xd->recon_left[1] += 8;
              xd->recon_left[2] += 8;
 }

 if (pbi->common.filter_level)
 {
 int skip_lf = (xd->mode_info_context->mbmi.mode != B_PRED &&
                               xd->mode_info_context->mbmi.mode != SPLITMV &&
                               xd->mode_info_context->mbmi.mb_skip_coeff);

 const int mode_index = lfi_n->mode_lf_lut[xd->mode_info_context->mbmi.mode];
 const int seg = xd->mode_info_context->mbmi.segment_id;
 const int ref_frame = xd->mode_info_context->mbmi.ref_frame;

               filter_level = lfi_n->lvl[seg][ref_frame][mode_index];

 if( mb_row != pc->mb_rows-1 )
 {
  
                   memcpy((pbi->mt_yabove_row[mb_row + 1] + 32 + mb_col*16), (xd->dst.y_buffer + 15 * recon_y_stride), 16);
                   memcpy((pbi->mt_uabove_row[mb_row + 1] + 16 + mb_col*8), (xd->dst.u_buffer + 7 * recon_uv_stride), 8);
                   memcpy((pbi->mt_vabove_row[mb_row + 1] + 16 + mb_col*8), (xd->dst.v_buffer + 7 * recon_uv_stride), 8);
 }

  
 if(mb_col != pc->mb_cols-1)
 {
                   MODE_INFO *next = xd->mode_info_context +1;

 if (next->mbmi.ref_frame == INTRA_FRAME)
 {
 for (i = 0; i < 16; i++)
                           pbi->mt_yleft_col[mb_row][i] = xd->dst.y_buffer [i* recon_y_stride + 15];
 for (i = 0; i < 8; i++)
 {
                           pbi->mt_uleft_col[mb_row][i] = xd->dst.u_buffer [i* recon_uv_stride + 7];
                           pbi->mt_vleft_col[mb_row][i] = xd->dst.v_buffer [i* recon_uv_stride + 7];
 }
 }
 }

  
 if (filter_level)
 {
 if(pc->filter_type == NORMAL_LOOPFILTER)
 {
                       loop_filter_info lfi;
                       FRAME_TYPE frame_type = pc->frame_type;
 const int hev_index = lfi_n->hev_thr_lut[frame_type][filter_level];
                       lfi.mblim = lfi_n->mblim[filter_level];
                       lfi.blim = lfi_n->blim[filter_level];
                       lfi.lim = lfi_n->lim[filter_level];
                       lfi.hev_thr = lfi_n->hev_thr[hev_index];

 if (mb_col > 0)
                           vp8_loop_filter_mbv
 (xd->dst.y_buffer, xd->dst.u_buffer, xd->dst.v_buffer, recon_y_stride, recon_uv_stride, &lfi);

 if (!skip_lf)
                           vp8_loop_filter_bv
 (xd->dst.y_buffer, xd->dst.u_buffer, xd->dst.v_buffer, recon_y_stride, recon_uv_stride, &lfi);

  
 if (mb_row > 0)
                           vp8_loop_filter_mbh
 (xd->dst.y_buffer, xd->dst.u_buffer, xd->dst.v_buffer, recon_y_stride, recon_uv_stride, &lfi);

 if (!skip_lf)
                           vp8_loop_filter_bh
 (xd->dst.y_buffer, xd->dst.u_buffer, xd->dst.v_buffer,  recon_y_stride, recon_uv_stride, &lfi);
 }
 else
 {
 if (mb_col > 0)
                           vp8_loop_filter_simple_mbv
 (xd->dst.y_buffer, recon_y_stride, lfi_n->mblim[filter_level]);

 if (!skip_lf)
                           vp8_loop_filter_simple_bv
 (xd->dst.y_buffer, recon_y_stride, lfi_n->blim[filter_level]);

  
 if (mb_row > 0)
                           vp8_loop_filter_simple_mbh
 (xd->dst.y_buffer, recon_y_stride, lfi_n->mblim[filter_level]);

 if (!skip_lf)
                           vp8_loop_filter_simple_bh
 (xd->dst.y_buffer, recon_y_stride, lfi_n->blim[filter_level]);
 }
 }

 }

           recon_yoffset += 16;
           recon_uvoffset += 8;

 ++xd->mode_info_context;  

           xd->above_context++;
 }

  
 if (pbi->common.filter_level)
 {
 if(mb_row != pc->mb_rows-1)
 {
 int lasty = yv12_fb_lst->y_width + VP8BORDERINPIXELS;
 int lastuv = (yv12_fb_lst->y_width>>1) + (VP8BORDERINPIXELS>>1);

 for (i = 0; i < 4; i++)
 {
                   pbi->mt_yabove_row[mb_row +1][lasty + i] = pbi->mt_yabove_row[mb_row +1][lasty -1];
                   pbi->mt_uabove_row[mb_row +1][lastuv + i] = pbi->mt_uabove_row[mb_row +1][lastuv -1];
                   pbi->mt_vabove_row[mb_row +1][lastuv + i] = pbi->mt_vabove_row[mb_row +1][lastuv -1];
 }
 }
 }
 else
           vp8_extend_mb_row(yv12_fb_new, xd->dst.y_buffer + 16,
                             xd->dst.u_buffer + 8, xd->dst.v_buffer + 8);

  
 *current_mb_col = mb_col + nsync;

 ++xd->mode_info_context;  
       xd->up_available = 1;

  
       xd->mode_info_context += xd->mode_info_stride * pbi->decoding_thread_count;
 }

  
 if (last_mb_row == (pc->mb_rows - 1))
        sem_post(&pbi->h_event_end_decoding);

}
