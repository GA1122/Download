static void free_seg_map(VP9_COMMON *cm) {
 int i;

 for (i = 0; i < NUM_PING_PONG_BUFFERS; ++i) {
    vpx_free(cm->seg_map_array[i]);
    cm->seg_map_array[i] = NULL;
 }

  cm->current_frame_seg_map = NULL;

 if (!cm->frame_parallel_decode) {
    cm->last_frame_seg_map = NULL;
 }
}
