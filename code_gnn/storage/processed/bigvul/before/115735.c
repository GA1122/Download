void ConnectionToClient::OnSessionStateChange(protocol::Session::State state) {
  DCHECK(message_loop_->BelongsToCurrentThread());

  DCHECK(handler_);
  switch(state) {
    case protocol::Session::CONNECTING:
      break;

    case protocol::Session::CONNECTED:
      video_writer_.reset(
          VideoWriter::Create(message_loop_, session_->config()));
      video_writer_->Init(
          session_.get(), base::Bind(&ConnectionToClient::OnVideoInitialized,
                                     base::Unretained(this)));
      break;

    case protocol::Session::CONNECTED_CHANNELS:
      client_control_sender_.reset(
          new ClientControlSender(message_loop_, session_->control_channel()));
      dispatcher_.reset(new HostMessageDispatcher());
      dispatcher_->Initialize(this, host_stub_, input_stub_);

      control_connected_ = true;
      input_connected_ = true;
      NotifyIfChannelsReady();
      break;

    case protocol::Session::CLOSED:
      CloseChannels();
      handler_->OnConnectionClosed(this);
      break;

    case protocol::Session::FAILED:
      CloseOnError();
      break;

    default:
      NOTREACHED();
  }
}
