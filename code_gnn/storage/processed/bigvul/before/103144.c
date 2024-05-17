void Browser::OpenBookmarkManagerWindow(Profile* profile) {
  Browser* browser = Browser::Create(profile);
  browser->ShowBookmarkManagerTab();
  browser->window()->Show();
}
