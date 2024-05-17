void Browser::RegisterIntentHandler(TabContents* tab,
                                    const string16& action,
                                    const string16& type,
                                    const string16& href,
                                    const string16& title) {
  RegisterIntentHandlerHelper(tab, action, type, href, title);
}
