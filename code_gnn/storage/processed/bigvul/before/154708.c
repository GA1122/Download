error::Error GLES2DecoderPassthroughImpl::DoGetString(GLenum name,
                                                      uint32_t bucket_id) {
  std::string extensions;
  const char* str = nullptr;

  switch (name) {
    case GL_VERSION:
      str = GetServiceVersionString(feature_info_.get());
      break;
    case GL_SHADING_LANGUAGE_VERSION:
      str = GetServiceShadingLanguageVersionString(feature_info_.get());
      break;
    case GL_EXTENSIONS: {
      extensions = gfx::MakeExtensionString(feature_info_->extensions());
      str = extensions.c_str();
      break;
    }
    default:
      str = reinterpret_cast<const char*>(api()->glGetStringFn(name));
      break;
  }

  Bucket* bucket = CreateBucket(bucket_id);
  bucket->SetFromString(str);
  return error::kNoError;
}
