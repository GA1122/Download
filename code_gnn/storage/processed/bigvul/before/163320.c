  explicit FrameFactoryImpl(const service_manager::BindSourceInfo& source_info)
      : source_info_(source_info), routing_id_highmark_(-1) {}
