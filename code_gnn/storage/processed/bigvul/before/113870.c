void SelectFileDialogImpl::SelectFileImpl(
    Type type,
    const string16& title,
    const FilePath& default_path,
    const FileTypeInfo* file_types,
    int file_type_index,
    const FilePath::StringType& default_extension,
    gfx::NativeWindow owning_window,
    void* params) {
  has_multiple_file_type_choices_ =
      file_types ? file_types->extensions.size() > 1 : true;
  ExecuteSelectParams execute_params(type, UTF16ToWide(title), default_path,
                                     file_types, file_type_index,
                                     default_extension, BeginRun(owning_window),
                                     owning_window, params);
  execute_params.run_state.dialog_thread->message_loop()->PostTask(
      FROM_HERE,
      base::Bind(&SelectFileDialogImpl::ExecuteSelectFile, this,
                 execute_params));
}
