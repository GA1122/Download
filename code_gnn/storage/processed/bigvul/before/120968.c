void SocketStream::RestartWithAuth(const AuthCredentials& credentials) {
  DCHECK(base::MessageLoop::current())
      << "The current base::MessageLoop must exist";
  DCHECK_EQ(base::MessageLoop::TYPE_IO, base::MessageLoop::current()->type())
      << "The current base::MessageLoop must be TYPE_IO";
  DCHECK(proxy_auth_controller_.get());
  if (!socket_.get()) {
    DVLOG(1) << "Socket is closed before restarting with auth.";
    return;
  }

  proxy_auth_controller_->ResetAuth(credentials);

  base::MessageLoop::current()->PostTask(
      FROM_HERE, base::Bind(&SocketStream::DoRestartWithAuth, this));
}
