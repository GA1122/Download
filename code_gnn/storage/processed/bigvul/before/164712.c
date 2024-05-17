  void WaitForMessage(int32_t* out_routing_id,
                      FrameMsg_PostMessage_Params* out_params) {
    run_loop_.Run();
    *out_routing_id = intercepted_routing_id_;
    *out_params = intercepted_params_;
  }
