void Browser::Search() {
  if (StartsWithASCII(GetSelectedTabContents()->GetURL().spec(),
                       chrome::kChromeUINewTabURL, true)) {
    CloseTab();
    return;
  }

  if (window_->IsFullscreen()) {
    ToggleFullscreenMode();
    MessageLoop::current()->PostTask(
        FROM_HERE, method_factory_.NewRunnableMethod(&Browser::Search));
    return;
  }

  NewTab();
}
