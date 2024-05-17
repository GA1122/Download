SelectFileDialogImpl::SelectFileDialogImpl(Listener* listener)
    : SelectFileDialog(listener),
      BaseShellDialogImpl(),
      has_multiple_file_type_choices_(false) {
}
