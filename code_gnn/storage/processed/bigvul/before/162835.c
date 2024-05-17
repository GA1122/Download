void ContextState::UpdateUnpackParameters() const {
  if (!feature_info_->IsES3Capable())
    return;
  if (bound_pixel_unpack_buffer.get()) {
    api()->glPixelStoreiFn(GL_UNPACK_ROW_LENGTH, unpack_row_length);
    api()->glPixelStoreiFn(GL_UNPACK_IMAGE_HEIGHT, unpack_image_height);
  } else {
    api()->glPixelStoreiFn(GL_UNPACK_ROW_LENGTH, 0);
    api()->glPixelStoreiFn(GL_UNPACK_IMAGE_HEIGHT, 0);
  }
}
