void SocketStream::DoAuthRequired() {
  if (delegate_ && proxy_auth_controller_.get())
    delegate_->OnAuthRequired(this, proxy_auth_controller_->auth_info().get());
  else
    DoLoop(ERR_UNEXPECTED);
}
