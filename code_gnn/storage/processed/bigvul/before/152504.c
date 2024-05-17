bool RenderFrameImpl::RunModalBeforeUnloadDialog(bool is_reload) {
  if (suppress_further_dialogs_)
    return false;

  bool success = false;
  base::string16 ignored_result;
  Send(new FrameHostMsg_RunBeforeUnloadConfirm(routing_id_, is_reload, &success,
                                               &ignored_result));
  return success;
}
