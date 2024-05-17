void GLES2Implementation::SetColorSpaceMetadataCHROMIUM(
    GLuint texture_id,
    GLColorSpace color_space) {
#if defined(__native_client__)
  SetGLError(GL_INVALID_VALUE, "GLES2::SetColorSpaceMetadataCHROMIUM",
             "not supported");
#else
  gfx::ColorSpace* gfx_color_space =
      reinterpret_cast<gfx::ColorSpace*>(color_space);
  base::Pickle color_space_data;
  IPC::ParamTraits<gfx::ColorSpace>::Write(&color_space_data, *gfx_color_space);

  ScopedTransferBufferPtr buffer(color_space_data.size(), helper_,
                                 transfer_buffer_);
  if (!buffer.valid() || buffer.size() < color_space_data.size()) {
    SetGLError(GL_OUT_OF_MEMORY, "GLES2::SetColorSpaceMetadataCHROMIUM",
               "out of memory");
    return;
  }
  memcpy(buffer.address(), color_space_data.data(), color_space_data.size());
  helper_->SetColorSpaceMetadataCHROMIUM(
      texture_id, buffer.shm_id(), buffer.offset(), color_space_data.size());
#endif
}
