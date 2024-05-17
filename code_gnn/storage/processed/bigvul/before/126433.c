void BrowserWindowGtk::ShowCreateChromeAppShortcutsDialog(
    Profile* profile, const extensions::Extension* app) {
  CreateChromeApplicationShortcutsDialogGtk::Show(window_, profile, app);
}
