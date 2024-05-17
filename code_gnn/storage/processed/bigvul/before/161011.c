void ChromeClientImpl::Show(NavigationPolicy navigation_policy) {
  if (web_view_->Client()) {
    web_view_->Client()->Show(
        static_cast<WebNavigationPolicy>(navigation_policy));
  }
}
