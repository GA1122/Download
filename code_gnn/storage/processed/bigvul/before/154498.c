void RequestExtensions(gl::GLApi* api,
                       const gfx::ExtensionSet& requestable_extensions,
                       const char* const* extensions_to_request,
                       size_t count) {
  for (size_t i = 0; i < count; i++) {
    if (gfx::HasExtension(requestable_extensions, extensions_to_request[i])) {
      api->glRequestExtensionANGLEFn(extensions_to_request[i]);
    }
  }
}
