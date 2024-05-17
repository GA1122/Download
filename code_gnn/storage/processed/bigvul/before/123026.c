void RenderWidgetHostImpl::ParentChanged(gfx::NativeViewId new_parent) {
#if defined(OS_WIN)
  HWND hwnd = reinterpret_cast<HWND>(new_parent);
  if (!hwnd)
    hwnd = WebPluginDelegateImpl::GetDefaultWindowParent();
  for (std::list<HWND>::iterator i = dummy_windows_for_activation_.begin();
        i != dummy_windows_for_activation_.end(); ++i) {
    SetParent(*i, hwnd);
  }
 #endif
 }
