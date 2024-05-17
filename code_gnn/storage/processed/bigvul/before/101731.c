void Browser::OpenClearBrowsingDataDialogWindow(Profile* profile) {
  Browser* browser = Browser::Create(profile);
  browser->OpenClearBrowsingDataDialog();
  browser->window()->Show();
}
