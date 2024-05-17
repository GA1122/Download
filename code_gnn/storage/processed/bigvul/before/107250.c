void AutomationProvider::JavaScriptStressTestControl(int tab_handle,
                                                     int cmd,
                                                     int param) {
  RenderViewHost* view = GetViewForTab(tab_handle);
  if (!view) {
    NOTREACHED();
    return;
  }

  view->JavaScriptStressTestControl(cmd, param);
}
