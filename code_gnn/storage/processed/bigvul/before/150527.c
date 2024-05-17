  void SetConfig(const ClientConfig& config) {
    config_run_loop_ = std::make_unique<base::RunLoop>();
     config_ = config;
   }
