void WebContentsViewAura::StartDragging(
    const DropData& drop_data,
    blink::WebDragOperationsMask operations,
    const gfx::ImageSkia& image,
    const gfx::Vector2d& image_offset,
    const DragEventSourceInfo& event_info) {
  aura::Window* root_window = GetNativeView()->GetRootWindow();
  if (!aura::client::GetDragDropClient(root_window)) {
    web_contents_->SystemDragEnded();
    return;
  }

  if (touch_editable_)
    touch_editable_->EndTouchEditing();

  ui::OSExchangeData::Provider* provider = ui::OSExchangeData::CreateProvider();
  PrepareDragData(drop_data, provider, web_contents_);

  ui::OSExchangeData data(provider);   

  if (!image.isNull()) {
    drag_utils::SetDragImageOnDataObject(image,
        gfx::Size(image.width(), image.height()), image_offset, &data);
  }

  scoped_ptr<WebDragSourceAura> drag_source(
      new WebDragSourceAura(GetNativeView(), web_contents_));

  int result_op = 0;
  {
    gfx::NativeView content_native_view = GetContentNativeView();
    base::MessageLoop::ScopedNestableTaskAllower allow(
        base::MessageLoop::current());
    result_op = aura::client::GetDragDropClient(root_window)
        ->StartDragAndDrop(data,
                           root_window,
                           content_native_view,
                           event_info.event_location,
                           ConvertFromWeb(operations),
                           event_info.event_source);
  }

  if (!drag_source->window()) {
    return;
  }

  EndDrag(ConvertToWeb(result_op));
  web_contents_->SystemDragEnded();
}
