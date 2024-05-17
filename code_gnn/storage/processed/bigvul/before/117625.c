DefaultBrowserInfoBarDelegate::DefaultBrowserInfoBarDelegate(
    InfoBarTabHelper* infobar_helper,
    PrefService* prefs)
    : ConfirmInfoBarDelegate(infobar_helper),
      prefs_(prefs),
      action_taken_(false),
      should_expire_(false),
      ALLOW_THIS_IN_INITIALIZER_LIST(weak_factory_(this)) {
  MessageLoop::current()->PostDelayedTask(
      FROM_HERE, base::Bind(&DefaultBrowserInfoBarDelegate::AllowExpiry,
                            weak_factory_.GetWeakPtr()),
      8000);   
}
