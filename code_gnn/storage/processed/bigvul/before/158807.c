void Browser::AddNewContents(WebContents* source,
                             std::unique_ptr<WebContents> new_contents,
                             WindowOpenDisposition disposition,
                             const gfx::Rect& initial_rect,
                             bool user_gesture,
                             bool* was_blocked) {
#if defined(OS_MACOSX)
  if (disposition == WindowOpenDisposition::NEW_POPUP &&
      exclusive_access_manager_->fullscreen_controller()
          ->IsFullscreenForBrowser()) {
    disposition = WindowOpenDisposition::NEW_FOREGROUND_TAB;
  }
#endif

  if (source && PopupBlockerTabHelper::ConsiderForPopupBlocking(disposition))
    PopupTracker::CreateForWebContents(new_contents.get(), source);
  chrome::AddWebContents(this, source, std::move(new_contents), disposition,
                         initial_rect);
}
