void WebPluginDelegateProxy::CopyFromBackBufferToFrontBuffer(
    const gfx::Rect& rect) {
#if defined(OS_MACOSX)
  const size_t stride =
      skia::PlatformCanvas::StrideForWidth(plugin_rect_.width());
  const size_t chunk_size = 4 * rect.width();
  DCHECK(back_buffer_dib() != NULL);
  uint8* source_data = static_cast<uint8*>(back_buffer_dib()->memory()) +
                       rect.y() * stride + 4 * rect.x();
  DCHECK(front_buffer_dib() != NULL);
  uint8* target_data = static_cast<uint8*>(front_buffer_dib()->memory()) +
                       rect.y() * stride + 4 * rect.x();
  for (int row = 0; row < rect.height(); ++row) {
    memcpy(target_data, source_data, chunk_size);
    source_data += stride;
    target_data += stride;
  }
#else
  BlitCanvasToCanvas(front_buffer_canvas(),
                     rect,
                     back_buffer_canvas(),
                     rect.origin());
#endif
}
