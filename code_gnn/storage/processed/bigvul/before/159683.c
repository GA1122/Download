scoped_refptr<shell_integration::DefaultProtocolClientWorker> CreateShellWorker(
    const shell_integration::DefaultWebClientWorkerCallback& callback,
    const std::string& protocol,
    ExternalProtocolHandler::Delegate* delegate) {
  if (delegate)
    return delegate->CreateShellWorker(callback, protocol);
  return base::MakeRefCounted<shell_integration::DefaultProtocolClientWorker>(
      callback, protocol);
}
