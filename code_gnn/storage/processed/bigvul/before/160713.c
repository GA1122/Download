bool RenderFrameImpl::RunFileChooser(
    const blink::WebFileChooserParams& params,
    blink::WebFileChooserCompletion* chooser_completion) {
  FileChooserParams ipc_params;
  if (params.directory)
    ipc_params.mode = FileChooserParams::UploadFolder;
  else if (params.multi_select)
    ipc_params.mode = FileChooserParams::OpenMultiple;
  else if (params.save_as)
    ipc_params.mode = FileChooserParams::Save;
  else
    ipc_params.mode = FileChooserParams::Open;
  ipc_params.title = params.title.Utf16();
  ipc_params.accept_types.reserve(params.accept_types.size());
  for (const auto& type : params.accept_types)
    ipc_params.accept_types.push_back(type.Utf16());
  ipc_params.need_local_path = params.need_local_path;
#if defined(OS_ANDROID)
  ipc_params.capture = params.use_media_capture;
#endif
  ipc_params.requestor = params.requestor;

  return ScheduleFileChooser(ipc_params, chooser_completion);
}
