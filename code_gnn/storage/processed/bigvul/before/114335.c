    renderbufferStorageMultisampleCHROMIUM(
        WGC3Denum target, WGC3Dsizei samples, WGC3Denum internalformat,
        WGC3Dsizei width, WGC3Dsizei height) {
  gl_->RenderbufferStorageMultisampleEXT(
      target, samples, internalformat, width, height);
}
