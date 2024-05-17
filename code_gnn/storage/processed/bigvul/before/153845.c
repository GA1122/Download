void GLES2Implementation::UpdateCachedExtensionsIfNeeded() {
  if (cached_extension_string_) {
    return;
  }
  GetStringHelper(GL_EXTENSIONS);
}
