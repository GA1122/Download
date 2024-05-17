 void NavigationController::UpdateVirtualURLToURL(
    NavigationEntry* entry, const GURL& new_url) {
  GURL new_virtual_url(new_url);
  if (BrowserURLHandler::GetInstance()->ReverseURLRewrite(
          &new_virtual_url, entry->virtual_url(), browser_context_)) {
    entry->set_virtual_url(new_virtual_url);
  }
}
