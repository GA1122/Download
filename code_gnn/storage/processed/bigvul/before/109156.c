bool RenderViewImpl::runModalBeforeUnloadDialog(
    WebFrame* frame, const WebString& message) {
  if (is_swapped_out_)
    return true;

  bool is_reload = false;
  WebDataSource* ds = frame->provisionalDataSource();
  if (ds)
    is_reload = (ds->navigationType() == WebKit::WebNavigationTypeReload);

  bool success = false;
  string16 ignored_result;
  SendAndRunNestedMessageLoop(new ViewHostMsg_RunBeforeUnloadConfirm(
      routing_id_, frame->document().url(), message, is_reload,
      &success, &ignored_result));
  return success;
}
