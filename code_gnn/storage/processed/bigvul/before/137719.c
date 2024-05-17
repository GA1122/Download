void ShowWarningMessageBox(const base::string16& message) {
  static bool is_dialog_shown = false;
  if (is_dialog_shown)
    return;
  base::AutoReset<bool> auto_reset(&is_dialog_shown, true);

  chrome::ShowWarningMessageBox(nullptr, base::string16(), message);
}
