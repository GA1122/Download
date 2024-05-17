bool RenderView::runModalBeforeUnloadDialog(
    WebFrame* frame, const WebString& message) {
  if (is_swapped_out_)
    return true;

  bool success = false;
  string16 ignored_result;
  SendAndRunNestedMessageLoop(new ViewHostMsg_RunBeforeUnloadConfirm(
      routing_id_, frame->document().url(), message,
      &success, &ignored_result));
  return success;
}
