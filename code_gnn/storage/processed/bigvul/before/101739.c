void Browser::OpenHelpWindow(Profile* profile) {
  Browser* browser = Browser::Create(profile);
  browser->ShowHelpTab();
  browser->window()->Show();
}
