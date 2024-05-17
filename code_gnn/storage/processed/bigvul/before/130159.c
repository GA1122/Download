void RenderFrameHostImpl::ExecuteJavaScript(
    const base::string16& javascript) {
  Send(new FrameMsg_JavaScriptExecuteRequest(routing_id_,
                                             javascript,
                                             0, false));
}
