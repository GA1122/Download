content::OpenURLParams GetOpenParams(const char* url) {
  return content::OpenURLParams(GURL(url), content::Referrer(),
                                WindowOpenDisposition::NEW_FOREGROUND_TAB,
                                ui::PAGE_TRANSITION_LINK, false);
}
