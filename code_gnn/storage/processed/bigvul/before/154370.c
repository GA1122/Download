bool GLES2DecoderImpl::SupportsDrawBuffers() const {
  return feature_info_->IsWebGL1OrES2Context() ?
      feature_info_->feature_flags().ext_draw_buffers : true;
}
