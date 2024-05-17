bool RenderFrameImpl::HandleCurrentKeyboardEvent() {
  bool did_execute_command = false;
  for (auto command : GetRenderWidget()->edit_commands()) {
    if (!frame_->ExecuteCommand(blink::WebString::FromUTF8(command.name),
                                blink::WebString::FromUTF8(command.value)))
      break;
    did_execute_command = true;
  }

  return did_execute_command;
}
