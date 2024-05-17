void DXVAVideoDecodeAccelerator::DXVAPictureBuffer::ReusePictureBuffer() {
  DCHECK(decoding_surface_);
  eglReleaseTexImage(
    static_cast<EGLDisplay*>(eglGetDisplay(EGL_DEFAULT_DISPLAY)),
    decoding_surface_,
    EGL_BACK_BUFFER);
  set_available(true);
}
