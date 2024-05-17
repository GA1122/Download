DEFINE_TRACE(WebLocalFrameImpl) {
  visitor->Trace(local_frame_client_);
  visitor->Trace(frame_);
  visitor->Trace(dev_tools_agent_);
  visitor->Trace(frame_widget_);
  visitor->Trace(text_finder_);
  visitor->Trace(print_context_);
  visitor->Trace(context_menu_node_);
  visitor->Trace(input_method_controller_);
  visitor->Trace(text_checker_client_);
  WebLocalFrameBase::Trace(visitor);
  WebFrame::TraceFrames(visitor, this);
}
