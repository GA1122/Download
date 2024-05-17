bool RenderView::runFileChooser(
    const WebKit::WebFileChooserParams& params,
    WebFileChooserCompletion* chooser_completion) {
  if (is_hidden())
    return false;
  ViewHostMsg_RunFileChooser_Params ipc_params;
  if (params.directory)
    ipc_params.mode = ViewHostMsg_RunFileChooser_Mode::OpenFolder;
  else if (params.multiSelect)
    ipc_params.mode = ViewHostMsg_RunFileChooser_Mode::OpenMultiple;
  else
    ipc_params.mode = ViewHostMsg_RunFileChooser_Mode::Open;
  ipc_params.title = params.title;
  ipc_params.default_file_name =
      webkit_glue::WebStringToFilePath(params.initialValue);
  ipc_params.accept_types = params.acceptTypes;

  return ScheduleFileChooser(ipc_params, chooser_completion);
}
