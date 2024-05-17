void RenderViewHostImpl::OnStartDragging(
    const WebDropData& drop_data,
    WebDragOperationsMask drag_operations_mask,
    const SkBitmap& bitmap,
    const gfx::Vector2d& bitmap_offset_in_dip,
    const DragEventSourceInfo& event_info) {
  RenderViewHostDelegateView* view = delegate_->GetDelegateView();
  if (!view)
    return;

  WebDropData filtered_data(drop_data);
  RenderProcessHost* process = GetProcess();
  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();

  if (!filtered_data.url.SchemeIs(chrome::kJavaScriptScheme))
    FilterURL(policy, process, true, &filtered_data.url);
  FilterURL(policy, process, false, &filtered_data.html_base_url);
  filtered_data.filenames.clear();
  for (std::vector<WebDropData::FileInfo>::const_iterator it =
           drop_data.filenames.begin();
       it != drop_data.filenames.end(); ++it) {
    FilePath path(FilePath::FromUTF8Unsafe(UTF16ToUTF8(it->path)));
    if (policy->CanReadFile(GetProcess()->GetID(), path))
      filtered_data.filenames.push_back(*it);
  }
  ui::ScaleFactor scale_factor = GetScaleFactorForView(GetView());
  gfx::ImageSkia image(gfx::ImageSkiaRep(bitmap, scale_factor));
  view->StartDragging(filtered_data, drag_operations_mask, image,
      bitmap_offset_in_dip, event_info);
}
