RefCountedMemory* NTPResourceCache::GetNewTabCSS(bool is_incognito) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  if (is_incognito) {
    if (!new_tab_incognito_css_.get())
      CreateNewTabIncognitoCSS();
  } else {
    if (!new_tab_css_.get())
      CreateNewTabCSS();
  }
  return is_incognito ? new_tab_incognito_css_.get()
                      : new_tab_css_.get();
}
