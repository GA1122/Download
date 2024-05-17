void Browser::OpenOptionsWindow(Profile* profile) {
  Browser* browser = Browser::Create(profile);
  browser->ShowOptionsTab(chrome::kDefaultOptionsSubPage);
  browser->window()->Show();
}
