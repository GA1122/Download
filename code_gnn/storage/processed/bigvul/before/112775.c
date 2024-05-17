void PrintPreviewHandler::SelectFile(const FilePath& default_filename) {
  ui::SelectFileDialog::FileTypeInfo file_type_info;
  file_type_info.extensions.resize(1);
  file_type_info.extensions[0].push_back(FILE_PATH_LITERAL("pdf"));

  if (!GetStickySettings()->save_path()) {
    base::ThreadRestrictions::ScopedAllowIO allow_io;
    FilePath file_path;
    PathService::Get(chrome::DIR_USER_DOCUMENTS, &file_path);
    GetStickySettings()->StoreSavePath(file_path);
  }

  select_file_dialog_ = ui::SelectFileDialog::Create(
      this, new ChromeSelectFilePolicy(preview_web_contents())),
  select_file_dialog_->SelectFile(
      ui::SelectFileDialog::SELECT_SAVEAS_FILE,
      string16(),
      GetStickySettings()->save_path()->Append(default_filename),
      &file_type_info,
      0,
      FILE_PATH_LITERAL(""),
      platform_util::GetTopLevel(preview_web_contents()->GetNativeView()),
      NULL);
}
