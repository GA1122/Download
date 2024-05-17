    DXVAVideoDecodeAccelerator::DXVAPictureBuffer::Create(
        const media::PictureBuffer& buffer, EGLConfig egl_config) {
  linked_ptr<DXVAPictureBuffer> picture_buffer(new DXVAPictureBuffer(buffer));

  EGLint attrib_list[] = {
    EGL_WIDTH, buffer.size().width(),
    EGL_HEIGHT, buffer.size().height(),
    EGL_TEXTURE_FORMAT, EGL_TEXTURE_RGB,
    EGL_TEXTURE_TARGET, EGL_TEXTURE_2D,
    EGL_NONE
  };

  picture_buffer->decoding_surface_ = eglCreatePbufferSurface(
      static_cast<EGLDisplay*>(eglGetDisplay(EGL_DEFAULT_DISPLAY)),
      egl_config,
      attrib_list);
  RETURN_ON_FAILURE(picture_buffer->decoding_surface_,
                    "Failed to create surface",
                    linked_ptr<DXVAPictureBuffer>(NULL));

  HANDLE share_handle = NULL;
  EGLBoolean ret = eglQuerySurfacePointerANGLE(
      static_cast<EGLDisplay*>(eglGetDisplay(EGL_DEFAULT_DISPLAY)),
      picture_buffer->decoding_surface_,
      EGL_D3D_TEXTURE_2D_SHARE_HANDLE_ANGLE,
      &share_handle);

  RETURN_ON_FAILURE(share_handle || ret != EGL_TRUE,
                    "Failed to query ANGLE surface pointer",
                    linked_ptr<DXVAPictureBuffer>(NULL));

  HRESULT hr = DXVAVideoDecodeAccelerator::device_->CreateTexture(
      buffer.size().width(),
      buffer.size().height(),
      1,
      D3DUSAGE_RENDERTARGET,
      D3DFMT_X8R8G8B8,
      D3DPOOL_DEFAULT,
      picture_buffer->decoding_texture_.Receive(),
      &share_handle);
  RETURN_ON_HR_FAILURE(hr, "Failed to create texture",
                       linked_ptr<DXVAPictureBuffer>(NULL));
  return picture_buffer;
}
