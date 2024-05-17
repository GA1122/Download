void RenderWidgetHostImpl::OnStartDragging(
    const DropData& drop_data,
    blink::WebDragOperationsMask drag_operations_mask,
    const SkBitmap& bitmap,
    const gfx::Vector2d& bitmap_offset_in_dip,
    const DragEventSourceInfo& event_info) {
  RenderViewHostDelegateView* view = delegate_->GetDelegateView();
  if (!view || !GetView()) {
    DragSourceSystemDragEnded();
    return;
  }

  DropData filtered_data(drop_data);
  RenderProcessHost* process = GetProcess();
  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();

  if (!filtered_data.url.SchemeIs(url::kJavaScriptScheme))
    process->FilterURL(true, &filtered_data.url);
  process->FilterURL(false, &filtered_data.html_base_url);
  filtered_data.filenames.clear();
  for (const auto& file_info : drop_data.filenames) {
    if (policy->CanReadFile(GetProcess()->GetID(), file_info.path))
      filtered_data.filenames.push_back(file_info);
  }

  storage::FileSystemContext* file_system_context =
      GetProcess()->GetStoragePartition()->GetFileSystemContext();
  filtered_data.file_system_files.clear();
  for (size_t i = 0; i < drop_data.file_system_files.size(); ++i) {
    storage::FileSystemURL file_system_url =
        file_system_context->CrackURL(drop_data.file_system_files[i].url);
    if (policy->CanReadFileSystemFile(GetProcess()->GetID(), file_system_url))
      filtered_data.file_system_files.push_back(drop_data.file_system_files[i]);
  }

  float scale = GetScaleFactorForView(GetView());
  gfx::ImageSkia image(gfx::ImageSkiaRep(bitmap, scale));
  view->StartDragging(filtered_data, drag_operations_mask, image,
                      bitmap_offset_in_dip, event_info, this);
}
