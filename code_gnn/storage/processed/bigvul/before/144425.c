 void ExtensionInstallDialogView::LinkClicked(views::Link* source,
                                             int event_flags) {
  GURL store_url(extension_urls::GetWebstoreItemDetailURLPrefix() +
                 prompt_->extension()->id());
  OpenURLParams params(
      store_url, Referrer(), NEW_FOREGROUND_TAB,
      ui::PAGE_TRANSITION_LINK,
      false);

  if (navigator_) {
    navigator_->OpenURL(params);
  } else {
    chrome::ScopedTabbedBrowserDisplayer displayer(
        profile_, chrome::GetActiveDesktop());
    displayer.browser()->OpenURL(params);
  }
  GetWidget()->Close();
}
