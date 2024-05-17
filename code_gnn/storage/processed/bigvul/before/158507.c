void WebLocalFrameImpl::Close() {
  WebLocalFrame::Close();

  client_ = nullptr;

  if (dev_tools_agent_)
    dev_tools_agent_.Clear();

  self_keep_alive_.Clear();

  if (print_context_)
    PrintEnd();
#if DCHECK_IS_ON()
  is_in_printing_ = false;
#endif
}
