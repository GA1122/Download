GLint GLES2DecoderImpl::ComputeMaxSamples() {
  GLint max_samples = 0;
  DoGetIntegerv(GL_MAX_SAMPLES, &max_samples, 1);

  if (feature_info_->IsWebGLContext() &&
      feature_info_->feature_flags().nv_internalformat_sample_query) {
    std::vector<GLint> temp;

    auto minWithSamplesForFormat = [&](GLenum internalformat) {
      temp.clear();
      InternalFormatSampleCountsHelper(GL_RENDERBUFFER, internalformat, &temp);
      max_samples = std::min(max_samples, temp[0]);
    };


    minWithSamplesForFormat(GL_RGBA8);
    minWithSamplesForFormat(GL_SRGB8_ALPHA8);
    minWithSamplesForFormat(GL_RGB10_A2);
    minWithSamplesForFormat(GL_RGBA4);
    minWithSamplesForFormat(GL_RGB5_A1);
    minWithSamplesForFormat(GL_RGB8);
    minWithSamplesForFormat(GL_RGB565);
    minWithSamplesForFormat(GL_RG8);
    minWithSamplesForFormat(GL_R8);
  }

  return max_samples;
}
