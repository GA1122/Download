void BrowserView::CreateLauncherIcon() {
#if defined(USE_ASH)
  if (chrome::IsNativeWindowInAsh(GetNativeWindow()) &&
      !launcher_item_controller_.get()) {
    launcher_item_controller_.reset(
        BrowserLauncherItemController::Create(browser_.get()));
  }
#endif   
}
