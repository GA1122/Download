void RenderViewImpl::OnFileChooserResponse(
    const std::vector<ui::SelectedFileInfo>& files) {
  if (file_chooser_completions_.empty())
    return;

  WebVector<WebFileChooserCompletion::SelectedFileInfo> selected_files(
      files.size());
  for (size_t i = 0; i < files.size(); ++i) {
    WebFileChooserCompletion::SelectedFileInfo selected_file;
    selected_file.path = webkit_base::FilePathToWebString(files[i].local_path);
    selected_file.displayName = webkit_base::FilePathStringToWebString(
        files[i].display_name);
    selected_files[i] = selected_file;
  }

  if (file_chooser_completions_.front()->completion)
    file_chooser_completions_.front()->completion->didChooseFile(
        selected_files);
  file_chooser_completions_.pop_front();

  if (!file_chooser_completions_.empty()) {
    Send(new ViewHostMsg_RunFileChooser(routing_id_,
        file_chooser_completions_.front()->params));
  }
}
