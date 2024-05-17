  ExtensionFunction::ResponseAction BluetoothSocketSendFunction::Run() {
    DCHECK_CURRENTLY_ON(work_thread_id());
  
  auto params = bluetooth_socket::Send::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(params.get());
//   params_ = bluetooth_socket::Send::Params::Create(*args_);
//   EXTENSION_FUNCTION_VALIDATE(params_.get());
  
  io_buffer_size_ = params->data.size();
  io_buffer_ = new net::WrappedIOBuffer(params->data.data());
//   io_buffer_size_ = params_->data.size();
//   io_buffer_ = new net::WrappedIOBuffer(params_->data.data());
  
  BluetoothApiSocket* socket = GetSocket(params->socket_id);
//   BluetoothApiSocket* socket = GetSocket(params_->socket_id);
    if (!socket)
      return RespondNow(Error(kSocketNotFoundError));
  
   socket->Send(io_buffer_,
                io_buffer_size_,
                base::Bind(&BluetoothSocketSendFunction::OnSuccess, this),
                base::Bind(&BluetoothSocketSendFunction::OnError, this));
   return did_respond() ? AlreadyResponded() : RespondLater();
 }