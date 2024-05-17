void Browser::OpenHelpWindow(Profile* profile) {
  Browser* browser = Browser::Create(profile);
  browser->OpenHelpTab();
  browser->window()->Show();
}
