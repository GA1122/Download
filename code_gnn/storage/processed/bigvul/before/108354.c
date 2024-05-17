AudioRendererHost::AudioEntry::AudioEntry()
    : render_view_id(0),
      stream_id(0),
      pending_buffer_request(false),
      pending_close(false) {
}
