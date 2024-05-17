  void Run() {
    win_util::SetAppIdForWindow(chrome::kBrowserAppID, window_);

    ScopedComPtr<ITaskbarList3> taskbar;
    HRESULT result = taskbar.CreateInstance(CLSID_TaskbarList, NULL,
                                            CLSCTX_INPROC_SERVER);
    if (FAILED(result)) {
      LOG(INFO) << "failed creating a TaskbarList object: " << result;
      return;
    }

    result = taskbar->HrInit();
    if (FAILED(result)) {
      LOG(INFO) << "failed initializing a TaskbarList obejct: " << result;
      return;
    }

    result = taskbar->RegisterTab(window_, frame_window_);
    if (FAILED(result)) {
      LOG(INFO) << "failed registering a thumbnail window: " << result;
      return;
    }

    result = taskbar->SetTabOrder(window_, NULL);
    if (FAILED(result)) {
      LOG(INFO) << "failed adding a thumbnail window: " << result;
      return;
    }

    if (active_) {
      result = taskbar->SetTabActive(window_, frame_window_, 0);
      if (FAILED(result))
        LOG(INFO) << "failed activating a thumbnail window: " << result;
    }
  }
