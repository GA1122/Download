void Browser::OpenFile() {
  content::RecordAction(UserMetricsAction("OpenFile"));
  select_file_dialog_ = ui::SelectFileDialog::Create(
      this, new ChromeSelectFilePolicy(
          chrome::GetActiveWebContents(this)));

  const FilePath directory = profile_->last_selected_directory();

  gfx::NativeWindow parent_window = window_->GetNativeWindow();
  ui::SelectFileDialog::FileTypeInfo file_types;
  file_types.support_gdata = true;
  select_file_dialog_->SelectFile(ui::SelectFileDialog::SELECT_OPEN_FILE,
                                  string16(), directory,
                                  &file_types, 0, FILE_PATH_LITERAL(""),
                                  parent_window, NULL);
}
