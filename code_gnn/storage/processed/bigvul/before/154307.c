GLsizei GLES2DecoderImpl::InternalFormatSampleCountsHelper(
    GLenum target,
    GLenum internalformat,
    std::vector<GLint>* out_sample_counts) {
  DCHECK(out_sample_counts == nullptr || out_sample_counts->size() == 0);

  GLint num_sample_counts = 0;
  if (gl_version_info().IsLowerThanGL(4, 2)) {
    if (GLES2Util::IsIntegerFormat(internalformat)) {
      return 0;
    }

    GLint max_samples = renderbuffer_manager()->max_samples();
    num_sample_counts = max_samples;

    if (out_sample_counts != nullptr) {
      out_sample_counts->reserve(num_sample_counts);
      for (GLint sample_count = max_samples; sample_count > 0; --sample_count) {
        out_sample_counts->push_back(sample_count);
      }
    }
  } else {
    api()->glGetInternalformativFn(target, internalformat, GL_NUM_SAMPLE_COUNTS,
                                   1, &num_sample_counts);

    bool remove_nonconformant_sample_counts =
        feature_info_->IsWebGLContext() &&
        feature_info_->feature_flags().nv_internalformat_sample_query;

    if (out_sample_counts != nullptr || remove_nonconformant_sample_counts) {
      std::vector<GLint> sample_counts(num_sample_counts);
      api()->glGetInternalformativFn(target, internalformat, GL_SAMPLES,
                                     num_sample_counts, sample_counts.data());

      if (remove_nonconformant_sample_counts) {
        ScopedGLErrorSuppressor suppressor(
            "GLES2DecoderImpl::InternalFormatSampleCountsHelper",
            error_state_.get());

        auto is_nonconformant = [this, target,
                                 internalformat](GLint sample_count) {
          GLint conformant = GL_FALSE;
          api()->glGetInternalformatSampleivNVFn(target, internalformat,
                                                 sample_count, GL_CONFORMANT_NV,
                                                 1, &conformant);

          if (api()->glGetErrorFn() != GL_NO_ERROR) {
            return sample_count > 8;
          }
          return conformant == GL_FALSE;
        };

        sample_counts.erase(
            std::remove_if(sample_counts.begin(), sample_counts.end(),
                           is_nonconformant),
            sample_counts.end());
        num_sample_counts = sample_counts.size();
      }

      if (out_sample_counts != nullptr) {
        *out_sample_counts = std::move(sample_counts);
      }
    }
  }

  DCHECK(out_sample_counts == nullptr ||
         out_sample_counts->size() == static_cast<size_t>(num_sample_counts));
  return num_sample_counts;
}
