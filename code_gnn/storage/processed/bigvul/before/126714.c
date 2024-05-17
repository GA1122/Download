void BrowserView::ActivateAppModalDialog() const {
  AppModalDialog* active_dialog =
      AppModalDialogQueue::GetInstance()->active_dialog();
  if (!active_dialog)
    return;

  Browser* modal_browser =
      browser::FindBrowserWithWebContents(active_dialog->web_contents());
  if (modal_browser && (browser_ != modal_browser)) {
    modal_browser->window()->FlashFrame(true);
    modal_browser->window()->Activate();
  }

  AppModalDialogQueue::GetInstance()->ActivateModalDialog();
}
