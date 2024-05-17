void BrowserView::ActivateAppModalDialog() const {
  app_modal::JavaScriptAppModalDialog* active_dialog =
      app_modal::AppModalDialogQueue::GetInstance()->active_dialog();
  if (!active_dialog)
    return;

  Browser* modal_browser =
      chrome::FindBrowserWithWebContents(active_dialog->web_contents());
  if (modal_browser && (browser_.get() != modal_browser)) {
    modal_browser->window()->FlashFrame(true);
    modal_browser->window()->Activate();
  }

  app_modal::AppModalDialogQueue::GetInstance()->ActivateModalDialog();
}
