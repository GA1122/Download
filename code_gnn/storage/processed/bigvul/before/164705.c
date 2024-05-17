  void OnOpenURL(const FrameHostMsg_OpenURL_Params& params) {
    intercepted_params_ = params;
  }
