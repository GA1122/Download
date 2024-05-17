void CheckAutoLaunchCallback() {
  if (!auto_launch_trial::IsInAutoLaunchGroup())
    return;

  Browser* browser = BrowserList::GetLastActive();
  TabContentsWrapper* tab = browser->GetSelectedTabContentsWrapper();

  InfoBarTabHelper* infobar_helper = tab->infobar_tab_helper();
  if (infobar_helper->infobar_count() > 0)
    return;

  infobar_helper->AddInfoBar(
      new AutolaunchInfoBarDelegate(infobar_helper,
      tab->profile()->GetPrefs()));
}
