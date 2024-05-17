int vp8dx_get_raw_frame(VP8D_COMP *pbi, YV12_BUFFER_CONFIG *sd, int64_t *time_stamp, int64_t *time_end_stamp, vp8_ppflags_t *flags)
{
 int ret = -1;

 if (pbi->ready_for_new_data == 1)
 return ret;

  
 if (pbi->common.show_frame == 0)
 return ret;

    pbi->ready_for_new_data = 1;
 *time_stamp = pbi->last_time_stamp;
 *time_end_stamp = 0;

#if CONFIG_POSTPROC
    ret = vp8_post_proc_frame(&pbi->common, sd, flags);
#else
 (void)flags;

 if (pbi->common.frame_to_show)
 {
 *sd = *pbi->common.frame_to_show;
        sd->y_width = pbi->common.Width;
        sd->y_height = pbi->common.Height;
        sd->uv_height = pbi->common.Height / 2;
        ret = 0;
 }
 else
 {
        ret = -1;
 }

#endif  
    vp8_clear_system_state();
 return ret;
}
