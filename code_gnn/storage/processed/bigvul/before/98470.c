void AeroPeekWindow::Destroy() {
  if (!IsWindow(hwnd()))
    return;

  ScopedComPtr<ITaskbarList3> taskbar;
  HRESULT result = taskbar.CreateInstance(CLSID_TaskbarList, NULL,
                                          CLSCTX_INPROC_SERVER);
  if (FAILED(result))
    return;

  result = taskbar->HrInit();
  if (FAILED(result))
    return;

  result = taskbar->UnregisterTab(hwnd());

  DestroyWindow(hwnd());
}
