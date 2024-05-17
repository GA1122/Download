void Browser::RegisterIntentHandlerHelper(TabContents* tab,
                                          const string16& action,
                                          const string16& type,
                                          const string16& href,
                                          const string16& title) {
  TabContentsWrapper* tcw = TabContentsWrapper::GetCurrentWrapperForContents(
      tab);
  if (!tcw || tcw->profile()->IsOffTheRecord())
    return;

  if (!CommandLine::ForCurrentProcess()->HasSwitch(switches::kEnableWebIntents))
    return;

  GURL service_url(href);
  if (!service_url.is_valid()) {
    const GURL& url = tab->GetURL();
    service_url = url.Resolve(href);
  }

  WebIntentData intent;
  intent.service_url = service_url;
  intent.action = action;
  intent.type = type;
  intent.title = title;
  tcw->infobar_tab_helper()->AddInfoBar(
      new RegisterIntentHandlerInfoBarDelegate(tab, intent));
}
