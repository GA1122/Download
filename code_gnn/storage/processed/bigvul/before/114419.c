    CopyOutputSampleDataToPictureBuffer(IDirect3DSurface9* dest_surface) {
  DCHECK(dest_surface);

  D3DSURFACE_DESC surface_desc;
  HRESULT hr = dest_surface->GetDesc(&surface_desc);
  RETURN_ON_HR_FAILURE(hr, "Failed to get surface description", false);

  D3DSURFACE_DESC texture_desc;
  decoding_texture_->GetLevelDesc(0, &texture_desc);
  if (texture_desc.Width != surface_desc.Width ||
      texture_desc.Height != surface_desc.Height) {
    NOTREACHED() << "Decode surface of different dimension than texture";
    return false;
  }

  hr = d3d9_->CheckDeviceFormatConversion(D3DADAPTER_DEFAULT,
                                          D3DDEVTYPE_HAL,
                                          surface_desc.Format,
                                          D3DFMT_X8R8G8B8);
  bool device_supports_format_conversion = (hr == S_OK);

  GLint current_texture = 0;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &current_texture);

  glBindTexture(GL_TEXTURE_2D, picture_buffer_.texture_id());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  if (device_supports_format_conversion) {
    base::win::ScopedComPtr<IDirect3DSurface9> d3d_surface;
    HRESULT hr = decoding_texture_->GetSurfaceLevel(0, d3d_surface.Receive());
    RETURN_ON_HR_FAILURE(hr, "Failed to get surface from texture", false);

    hr = device_->StretchRect(dest_surface,
                              NULL,
                              d3d_surface,
                              NULL,
                              D3DTEXF_NONE);
    RETURN_ON_HR_FAILURE(hr, "Colorspace conversion via StretchRect failed",
                         false);
    hr = query_->Issue(D3DISSUE_END);
    do {
      hr = query_->GetData(NULL, 0, D3DGETDATA_FLUSH);
      if (hr == S_FALSE)
        Sleep(0);
    } while (hr == S_FALSE);

    eglBindTexImage(
        static_cast<EGLDisplay*>(eglGetDisplay(EGL_DEFAULT_DISPLAY)),
        decoding_surface_,
        EGL_BACK_BUFFER);
  } else {
    scoped_array<char> bits;
    RETURN_ON_FAILURE(GetBitmapFromSurface(DXVAVideoDecodeAccelerator::device_,
                                           dest_surface, &bits),
                      "Failed to get bitmap from surface for rendering",
                      false);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA_EXT, surface_desc.Width,
                 surface_desc.Height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE,
                 reinterpret_cast<GLvoid*>(bits.get()));
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, current_texture);
  return true;
}
