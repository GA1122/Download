  CreateViewCommandBufferRequest(
      int32 surface_id_,
      const GPUCreateCommandBufferConfig& init_params_,
      IPC::Message* reply_)
      : surface_id(surface_id_),
        init_params(init_params_),
        reply(reply_) {
  }
