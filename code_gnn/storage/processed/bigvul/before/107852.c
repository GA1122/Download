bool PrintWebViewHelper::Send(IPC::Message* msg) {
  return render_view_->Send(msg);
}
