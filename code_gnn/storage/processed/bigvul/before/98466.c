HRESULT CallDwmSetIconicThumbnail(HWND window, HBITMAP bitmap, DWORD flags) {
  FilePath dwmapi_path(base::GetNativeLibraryName(L"dwmapi"));
  base::ScopedNativeLibrary dwmapi(dwmapi_path);

  typedef HRESULT (STDAPICALLTYPE *DwmSetIconicThumbnailProc)(
      HWND, HBITMAP, DWORD);
  DwmSetIconicThumbnailProc dwm_set_iconic_thumbnail =
      static_cast<DwmSetIconicThumbnailProc>(
      dwmapi.GetFunctionPointer("DwmSetIconicThumbnail"));

  if (!dwm_set_iconic_thumbnail)
    return E_FAIL;

  return dwm_set_iconic_thumbnail(window, bitmap, flags);
}
