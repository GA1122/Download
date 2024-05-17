void AutofillManager::OnShowAutofillDialog() {
  Browser* browser = BrowserList::GetLastActive();
  if (browser)
    browser->ShowOptionsTab(chrome::kAutofillSubPage);
}
