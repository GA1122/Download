void GLES2Implementation::InvalidateCachedExtensions() {
  cached_extension_string_ = nullptr;
  cached_extensions_.clear();
}
