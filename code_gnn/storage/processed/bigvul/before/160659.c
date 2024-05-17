void RenderFrameImpl::OnFileChooserResponse(
    const std::vector<content::FileChooserFileInfo>& files) {
  if (file_chooser_completions_.empty())
    return;

  WebVector<blink::WebFileChooserCompletion::SelectedFileInfo> selected_files(
      files.size());
  size_t current_size = 0;
  for (size_t i = 0; i < files.size(); ++i) {
    blink::WebFileChooserCompletion::SelectedFileInfo selected_file;
    selected_file.path = blink::FilePathToWebString(files[i].file_path);

    if (selected_file.path.IsEmpty())
      continue;

    selected_file.display_name =
        blink::FilePathToWebString(base::FilePath(files[i].display_name));
    if (files[i].file_system_url.is_valid()) {
      selected_file.file_system_url = files[i].file_system_url;
      selected_file.length = files[i].length;
      selected_file.modification_time = files[i].modification_time.ToDoubleT();
      selected_file.is_directory = files[i].is_directory;
    }

    selected_files[current_size] = selected_file;
    current_size++;
  }

  if (current_size < selected_files.size()) {
    WebVector<blink::WebFileChooserCompletion::SelectedFileInfo> truncated_list(
        selected_files.Data(), current_size);
    selected_files.Swap(truncated_list);
  }

  if (file_chooser_completions_.front()->completion) {
    file_chooser_completions_.front()->completion->DidChooseFile(
        selected_files);
  }
  file_chooser_completions_.pop_front();

  if (!file_chooser_completions_.empty()) {
    Send(new FrameHostMsg_RunFileChooser(
        routing_id_, file_chooser_completions_.front()->params));
  }
}
