  void OnRouteMessageEvent(const FrameMsg_PostMessage_Params& params) {
    intercepted_params_ = params;

    intercepted_params_.message->data.EnsureDataIsOwned();
  }
