void BrowserWindowGtk::UpdateTitleBar() {
  TRACE_EVENT0("ui::gtk", "BrowserWindowGtk::UpdateTitleBar");
  string16 title = browser_->GetWindowTitleForCurrentTab();
  gtk_window_set_title(window_, UTF16ToUTF8(title).c_str());
  if (ShouldShowWindowIcon())
    titlebar_->UpdateTitleAndIcon();
}
