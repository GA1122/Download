static bool GetBitmapFromSurface(IDirect3DDevice9Ex* device,
                                 IDirect3DSurface9* surface,
                                 scoped_array<char>* bits) {

  D3DSURFACE_DESC surface_desc;
  HRESULT hr = surface->GetDesc(&surface_desc);
  RETURN_ON_HR_FAILURE(hr, "Failed to get surface description", false);

  base::win::ScopedComPtr<IDirect3DSurface9> dest_surface;
  hr = device->CreateOffscreenPlainSurface(surface_desc.Width,
                                           surface_desc.Height,
                                           D3DFMT_A8R8G8B8,
                                           D3DPOOL_DEFAULT,
                                           dest_surface.Receive(),
                                           NULL);
  RETURN_ON_HR_FAILURE(hr, "Failed to create offscreen surface", false);

  hr = D3DXLoadSurfaceFromSurface(dest_surface, NULL, NULL, surface, NULL,
                                  NULL, D3DX_DEFAULT, 0);
  RETURN_ON_HR_FAILURE(hr, "D3DXLoadSurfaceFromSurface failed", false);

  HDC hdc = NULL;
  hr = dest_surface->GetDC(&hdc);
  RETURN_ON_HR_FAILURE(hr, "Failed to get HDC from surface", false);

  HBITMAP bitmap =
      reinterpret_cast<HBITMAP>(GetCurrentObject(hdc, OBJ_BITMAP));
  if (!bitmap) {
    NOTREACHED() << "Failed to get bitmap from DC";
    dest_surface->ReleaseDC(hdc);
    return false;
  }

  BITMAP bitmap_basic_info = {0};
  if (!GetObject(bitmap, sizeof(BITMAP), &bitmap_basic_info)) {
    NOTREACHED() << "Failed to read bitmap info";
    dest_surface->ReleaseDC(hdc);
    return false;
  }
  BITMAPINFO bitmap_info = {0};
  bitmap_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bitmap_info.bmiHeader.biWidth = bitmap_basic_info.bmWidth;
  bitmap_info.bmiHeader.biHeight = bitmap_basic_info.bmHeight;
  bitmap_info.bmiHeader.biPlanes = 1;
  bitmap_info.bmiHeader.biBitCount = bitmap_basic_info.bmBitsPixel;
  bitmap_info.bmiHeader.biCompression = BI_RGB;
  bitmap_info.bmiHeader.biSizeImage = 0;
  bitmap_info.bmiHeader.biClrUsed = 0;

  int ret = GetDIBits(hdc, bitmap, 0, 0, NULL, &bitmap_info, DIB_RGB_COLORS);
  if (!ret || bitmap_info.bmiHeader.biSizeImage <= 0) {
    NOTREACHED() << "Failed to read bitmap size";
    dest_surface->ReleaseDC(hdc);
    return false;
  }

  bits->reset(new char[bitmap_info.bmiHeader.biSizeImage]);
  ret = GetDIBits(hdc, bitmap, 0, bitmap_basic_info.bmHeight, bits->get(),
                  &bitmap_info, DIB_RGB_COLORS);
  if (!ret) {
    NOTREACHED() << "Failed to retrieve bitmap bits.";
  }
  dest_surface->ReleaseDC(hdc);
  return !!ret;
}
