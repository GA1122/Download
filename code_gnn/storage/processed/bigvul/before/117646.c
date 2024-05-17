LearnMoreInfoBar::LearnMoreInfoBar(InfoBarTabHelper* infobar_helper,
                                   const string16& message,
                                   const GURL& url)
    : LinkInfoBarDelegate(infobar_helper),
      message_(message),
      learn_more_url_(url) {
}
