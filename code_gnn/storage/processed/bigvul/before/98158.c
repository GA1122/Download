bool AutoFillCCInfoBarDelegate::LinkClicked(WindowOpenDisposition disposition) {
  host_->tab_contents()->OpenURL(GURL(kAutoFillLearnMoreUrl), GURL(),
                                 NEW_FOREGROUND_TAB, PageTransition::TYPED);
  return false;
}
