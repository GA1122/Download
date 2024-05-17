void PrintRenderFrameHelper::OnPrintForSystemDialog() {
  if (ipc_nesting_level_ > 1)
    return;
  blink::WebLocalFrame* frame = print_preview_context_.source_frame();
  if (!frame) {
    NOTREACHED();
    return;
  }
  auto weak_this = weak_ptr_factory_.GetWeakPtr();
  Print(frame, print_preview_context_.source_node(), false);
  if (weak_this)
    frame->DispatchAfterPrintEvent();
}
