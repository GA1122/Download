void AutoFillManager::OnShowAutoFillDialog() {
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableTabbedOptions)) {
    Browser* browser = BrowserList::GetLastActive();
    if (browser)
      browser->ShowOptionsTab(chrome::kAutoFillSubPage);
    return;
  }

  ShowAutoFillDialog(tab_contents_->GetContentNativeView(),
                     personal_data_,
                     tab_contents_->profile()->GetOriginalProfile());
}
