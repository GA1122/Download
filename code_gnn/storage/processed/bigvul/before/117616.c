AutolaunchInfoBarDelegate::AutolaunchInfoBarDelegate(
    InfoBarTabHelper* infobar_helper,
    PrefService* prefs)
    : ConfirmInfoBarDelegate(infobar_helper),
      prefs_(prefs),
      action_taken_(false),
      should_expire_(false),
      ALLOW_THIS_IN_INITIALIZER_LIST(weak_factory_(this)) {
  auto_launch_trial::UpdateInfobarShownMetric();

  int count = prefs_->GetInteger(prefs::kShownAutoLaunchInfobar);
  prefs_->SetInteger(prefs::kShownAutoLaunchInfobar, count + 1);

  MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      base::Bind(&AutolaunchInfoBarDelegate::AllowExpiry,
                 weak_factory_.GetWeakPtr()),
      8000);   
}
