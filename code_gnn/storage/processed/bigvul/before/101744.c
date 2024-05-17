void Browser::OpenInstantConfirmDialogWindow(Profile* profile) {
  Browser* browser = Browser::Create(profile);
  browser->OpenInstantConfirmDialog();
  browser->window()->Show();
}
