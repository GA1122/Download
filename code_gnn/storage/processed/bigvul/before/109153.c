bool RenderViewImpl::runFileChooser(
    const WebKit::WebFileChooserParams& params,
    WebFileChooserCompletion* chooser_completion) {
  if (is_hidden())
    return false;
  content::FileChooserParams ipc_params;
  if (params.directory)
    ipc_params.mode = content::FileChooserParams::OpenFolder;
  else if (params.multiSelect)
    ipc_params.mode = content::FileChooserParams::OpenMultiple;
  else if (params.saveAs)
    ipc_params.mode = content::FileChooserParams::Save;
  else
    ipc_params.mode = content::FileChooserParams::Open;
  ipc_params.title = params.title;
  ipc_params.default_file_name =
      webkit_glue::WebStringToFilePath(params.initialValue);
  ipc_params.accept_types.reserve(params.acceptTypes.size());
  for (size_t i = 0; i < params.acceptTypes.size(); ++i)
    ipc_params.accept_types.push_back(params.acceptTypes[i]);

  return ScheduleFileChooser(ipc_params, chooser_completion);
}
