base::RefCountedMemory* NTPResourceCache::GetNewTabCSS(WindowType win_type) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (win_type == GUEST) {
    if (!new_tab_guest_css_.get())
      CreateNewTabGuestCSS();
    return new_tab_guest_css_.get();
  } else if (win_type == INCOGNITO) {
    if (!new_tab_incognito_css_.get())
      CreateNewTabIncognitoCSS();
    return new_tab_incognito_css_.get();
  } else {
    if (!new_tab_css_.get())
      CreateNewTabCSS();
    return new_tab_css_.get();
  }
}
