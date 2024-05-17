HANDLE CreateDIB(HDC dc, int width, int height, int color_depth) {
  BITMAPV5HEADER hdr = {0};
  ZeroMemory(&hdr, sizeof(BITMAPV5HEADER));

  hdr.bV5Size = sizeof(BITMAPINFOHEADER);
  hdr.bV5Width = width;
  hdr.bV5Height = -height;   
  hdr.bV5Planes = 1;
  hdr.bV5BitCount = color_depth;
  hdr.bV5Compression = BI_RGB;   
  hdr.bV5SizeImage = 0;
  hdr.bV5XPelsPerMeter = 1;
  hdr.bV5YPelsPerMeter = 1;
  hdr.bV5ClrUsed = 0;
  hdr.bV5ClrImportant = 0;

  if (BackingStoreWin::ColorManagementEnabled()) {
    hdr.bV5CSType = LCS_sRGB;
    hdr.bV5Intent = LCS_GM_IMAGES;
  }

  void* data = NULL;
  HANDLE dib = CreateDIBSection(dc, reinterpret_cast<BITMAPINFO*>(&hdr),
                                0, &data, NULL, 0);
  DCHECK(data);
  return dib;
}
