void Browser::ProcessPendingTabs() {
  if (!is_attempting_to_close_browser_) {
    return;
  }

  if (HasCompletedUnloadProcessing()) {
    OnWindowClosing();
    return;
  }

  if (!tabs_needing_before_unload_fired_.empty()) {
    TabContents* tab = *(tabs_needing_before_unload_fired_.begin());
    if (tab->render_view_host()) {
      tab->render_view_host()->FirePageBeforeUnload(false);
    } else {
      ClearUnloadState(tab, true);
    }
  } else if (!tabs_needing_unload_fired_.empty()) {
    TabContents* tab = *(tabs_needing_unload_fired_.begin());
    if (tab->render_view_host()) {
      tab->render_view_host()->ClosePage();
    } else {
      ClearUnloadState(tab, true);
    }
  } else {
    NOTREACHED();
  }
}
