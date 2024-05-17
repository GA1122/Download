int WebContentsViewAura::OnDragUpdated(const ui::DropTargetEvent& event) {
  DCHECK(current_rvh_for_drag_);
  if (current_rvh_for_drag_ != web_contents_->GetRenderViewHost())
    OnDragEntered(event);

  blink::WebDragOperationsMask op = ConvertToWeb(event.source_operations());
  gfx::Point screen_pt =
      gfx::Screen::GetScreenFor(GetNativeView())->GetCursorScreenPoint();
  web_contents_->GetRenderViewHost()->DragTargetDragOver(
      event.location(), screen_pt, op,
      ConvertAuraEventFlagsToWebInputEventModifiers(event.flags()));

  if (drag_dest_delegate_)
    drag_dest_delegate_->OnDragOver();

  return ConvertFromWeb(current_drag_op_);
}
