bool TargetIsSupported(const FeatureInfo* feature_info, GLuint target) {
  switch (target) {
    case GL_TEXTURE_2D:
      return true;
    case GL_TEXTURE_CUBE_MAP:
      return true;
    case GL_TEXTURE_RECTANGLE_ARB:
      return feature_info->feature_flags().arb_texture_rectangle;
    case GL_TEXTURE_EXTERNAL_OES:
      return feature_info->feature_flags().oes_egl_image_external ||
             feature_info->feature_flags().nv_egl_stream_consumer_external;
    default:
      NOTREACHED();
      return false;
  }
}
