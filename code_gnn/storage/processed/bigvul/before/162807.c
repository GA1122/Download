void ContextState::PushTextureUnpackState() const {
  api()->glPixelStoreiFn(GL_UNPACK_ALIGNMENT, 1);

  if (bound_pixel_unpack_buffer.get()) {
    api()->glBindBufferFn(GL_PIXEL_UNPACK_BUFFER, 0);
    api()->glPixelStoreiFn(GL_UNPACK_ROW_LENGTH, 0);
    api()->glPixelStoreiFn(GL_UNPACK_IMAGE_HEIGHT, 0);
    DCHECK_EQ(0, unpack_skip_pixels);
    DCHECK_EQ(0, unpack_skip_rows);
    DCHECK_EQ(0, unpack_skip_images);
  }
}
