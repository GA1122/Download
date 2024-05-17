void BrowserView::DestroyBrowser() {
#if defined(OS_WIN) || (defined(OS_LINUX) && !defined(OS_CHROMEOS))
  GetWidget()->GetNativeView()->RemovePreTargetHandler(
      ConfirmQuitBubbleController::GetInstance());
#endif

  GetWidget()->RemoveObserver(this);
  frame_->CloseNow();
}
