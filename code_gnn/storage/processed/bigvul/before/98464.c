HRESULT CallDwmInvalidateIconicBitmaps(HWND window) {
  FilePath dwmapi_path(base::GetNativeLibraryName(L"dwmapi"));
  base::ScopedNativeLibrary dwmapi(dwmapi_path);

  typedef HRESULT (STDAPICALLTYPE *DwmInvalidateIconicBitmapsProc)(HWND);
  DwmInvalidateIconicBitmapsProc dwm_invalidate_iconic_bitmaps =
      static_cast<DwmInvalidateIconicBitmapsProc>(
      dwmapi.GetFunctionPointer("DwmInvalidateIconicBitmaps"));

  if (!dwm_invalidate_iconic_bitmaps)
    return E_FAIL;

  return dwm_invalidate_iconic_bitmaps(window);
}
