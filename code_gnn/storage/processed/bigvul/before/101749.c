void Browser::OpenOptionsWindow(Profile* profile) {
  Browser* browser = Browser::Create(profile);
  browser->OpenOptionsDialog();
  browser->window()->Show();
}
