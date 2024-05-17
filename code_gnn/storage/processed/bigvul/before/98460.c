void AeroPeekWindow::Activate() {
  tab_active_ = true;

  if (!IsWindow(hwnd())) {
    Update(false);
    return;
  }

  ScopedComPtr<ITaskbarList3> taskbar;
  HRESULT result = taskbar.CreateInstance(CLSID_TaskbarList, NULL,
                                          CLSCTX_INPROC_SERVER);
  if (FAILED(result)) {
    LOG(ERROR) << "failed creating an ITaskbarList3 interface.";
    return;
  }

  result = taskbar->HrInit();
  if (FAILED(result)) {
    LOG(ERROR) << "failed initializing an ITaskbarList3 interface.";
    return;
  }

  result = taskbar->ActivateTab(hwnd());
  if (FAILED(result)) {
    LOG(ERROR) << "failed activating a thumbnail window.";
    return;
  }

  UpdateThumbnail();
}
