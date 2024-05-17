void vp9_remove_common(VP9_COMMON *cm) {
  vp9_free_context_buffers(cm);

  vpx_free(cm->fc);
  cm->fc = NULL;
  vpx_free(cm->frame_contexts);
  cm->frame_contexts = NULL;
}
