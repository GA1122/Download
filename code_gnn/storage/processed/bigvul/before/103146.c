void Browser::OpenClearBrowingDataDialogWindow(Profile* profile) {
  Browser* browser = Browser::Create(profile);
  browser->OpenClearBrowsingDataDialog();
  browser->window()->Show();
}
