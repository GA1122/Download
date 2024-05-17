void ContextState::RestoreUnpackState() const {
  api()->glPixelStoreiFn(GL_UNPACK_ALIGNMENT, unpack_alignment);
  if (bound_pixel_unpack_buffer.get()) {
    api()->glBindBufferFn(GL_PIXEL_UNPACK_BUFFER,
                          GetBufferId(bound_pixel_unpack_buffer.get()));
    api()->glPixelStoreiFn(GL_UNPACK_ROW_LENGTH, unpack_row_length);
    api()->glPixelStoreiFn(GL_UNPACK_IMAGE_HEIGHT, unpack_image_height);
  }
}
