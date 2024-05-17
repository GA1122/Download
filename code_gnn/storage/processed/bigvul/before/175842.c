 void vp8mt_decode_mb_rows( VP8D_COMP *pbi, MACROBLOCKD *xd)
{
    VP8_COMMON *pc = &pbi->common;
 unsigned int i;
 int j;

 int filter_level = pc->filter_level;
    YV12_BUFFER_CONFIG *yv12_fb_new = pbi->dec_fb_ref[INTRA_FRAME];

 if (filter_level)
 {
  
        memset(pbi->mt_yabove_row[0] + VP8BORDERINPIXELS-1, 127, yv12_fb_new->y_width + 5);
        memset(pbi->mt_uabove_row[0] + (VP8BORDERINPIXELS>>1)-1, 127, (yv12_fb_new->y_width>>1) +5);
        memset(pbi->mt_vabove_row[0] + (VP8BORDERINPIXELS>>1)-1, 127, (yv12_fb_new->y_width>>1) +5);

 for (j=1; j<pc->mb_rows; j++)
 {
            memset(pbi->mt_yabove_row[j] + VP8BORDERINPIXELS-1, (unsigned char)129, 1);
            memset(pbi->mt_uabove_row[j] + (VP8BORDERINPIXELS>>1)-1, (unsigned char)129, 1);
            memset(pbi->mt_vabove_row[j] + (VP8BORDERINPIXELS>>1)-1, (unsigned char)129, 1);
 }

  
 for (j=0; j<pc->mb_rows; j++)
 {
            memset(pbi->mt_yleft_col[j], (unsigned char)129, 16);
            memset(pbi->mt_uleft_col[j], (unsigned char)129, 8);
            memset(pbi->mt_vleft_col[j], (unsigned char)129, 8);
 }

  
        vp8_loop_filter_frame_init(pc, &pbi->mb, filter_level);
 }
 else
        vp8_setup_intra_recon_top_line(yv12_fb_new);

    setup_decoding_thread_data(pbi, xd, pbi->mb_row_di, pbi->decoding_thread_count);

 for (i = 0; i < pbi->decoding_thread_count; i++)
        sem_post(&pbi->h_event_start_decoding[i]);

    mt_decode_mb_rows(pbi, xd, 0);

    sem_wait(&pbi->h_event_end_decoding);  
}
