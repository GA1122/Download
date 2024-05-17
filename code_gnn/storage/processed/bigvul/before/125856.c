WebRtcAudioRenderer::WebRtcAudioRenderer(int source_render_view_id)
    : state_(UNINITIALIZED),
      source_render_view_id_(source_render_view_id),
      source_(NULL) {
}
