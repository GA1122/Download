PepperPlatformAudioInput::PepperPlatformAudioInput()
    : client_(NULL),
      main_message_loop_proxy_(base::MessageLoopProxy::current()),
      io_message_loop_proxy_(ChildProcess::current()->io_message_loop_proxy()),
      render_frame_id_(MSG_ROUTING_NONE),
      create_stream_sent_(false),
      pending_open_device_(false),
      pending_open_device_id_(-1) {}
