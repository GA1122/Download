void ExtensionInstalledBubbleGtk::BubbleClosing(BubbleGtk* bubble,
                                                bool closed_by_escape) {
  if (extension_ && type_ == PAGE_ACTION) {
    BrowserWindowGtk* browser_window =
          BrowserWindowGtk::GetBrowserWindowForNativeWindow(
              browser_->window()->GetNativeHandle());
    LocationBarViewGtk* location_bar_view =
        browser_window->GetToolbar()->GetLocationBarView();
    location_bar_view->SetPreviewEnabledPageAction(extension_->page_action(),
                                                   false);   
  }

  MessageLoopForUI::current()->PostTask(
      FROM_HERE,
      base::Bind(&ExtensionInstalledBubbleGtk::Close, this));
}
