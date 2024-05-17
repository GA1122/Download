const GLubyte* GLES2Implementation::GetStringHelper(GLenum name) {
  if (name == GL_EXTENSIONS && cached_extension_string_) {
    return reinterpret_cast<const GLubyte*>(cached_extension_string_);
  }
  const char* result = nullptr;
  helper_->SetBucketSize(kResultBucketId, 0);
  helper_->GetString(name, kResultBucketId);
  std::string str;
  if (GetBucketAsString(kResultBucketId, &str)) {
    if (name == GL_EXTENSIONS) {
      str += std::string(str.empty() ? "" : " ") +
             "GL_CHROMIUM_image "
             "GL_CHROMIUM_map_sub "
             "GL_CHROMIUM_ordering_barrier "
             "GL_CHROMIUM_sync_point "
             "GL_EXT_unpack_subimage";
    }

    const std::string& cache = *gl_strings_.insert(str).first;
    result = cache.c_str();

    if (name == GL_EXTENSIONS) {
      cached_extension_string_ = result;
      std::vector<std::string> extensions =
          base::SplitString(cache, base::kWhitespaceASCII,
                            base::TRIM_WHITESPACE, base::SPLIT_WANT_NONEMPTY);
      for (const std::string& extension : extensions) {
        cached_extensions_.push_back(
            gl_strings_.insert(extension).first->c_str());
      }
    }
  }
  return reinterpret_cast<const GLubyte*>(result);
}
