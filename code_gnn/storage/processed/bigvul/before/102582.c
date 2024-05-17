NaClProcessHost::~NaClProcessHost() {
#ifndef DISABLE_NACL
  for (size_t i = 0; i < internal_->sockets_for_renderer.size(); i++) {
    if (nacl::Close(internal_->sockets_for_renderer[i]) != 0) {
      LOG(ERROR) << "nacl::Close() failed";
    }
  }
  for (size_t i = 0; i < internal_->sockets_for_sel_ldr.size(); i++) {
    if (nacl::Close(internal_->sockets_for_sel_ldr[i]) != 0) {
      LOG(ERROR) << "nacl::Close() failed";
    }
  }
#endif

  if (reply_msg_) {
    reply_msg_->set_reply_error();
    chrome_render_message_filter_->Send(reply_msg_);
  }
}
