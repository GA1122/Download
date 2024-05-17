TestDataReductionProxyConfig::TestDataReductionProxyConfig(
    std::unique_ptr<DataReductionProxyConfigValues> config_values,
    scoped_refptr<base::SingleThreadTaskRunner> io_task_runner,
    scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner,
    DataReductionProxyConfigurator* configurator)
    : DataReductionProxyConfig(
          io_task_runner,
          ui_task_runner,
          network::TestNetworkConnectionTracker::GetInstance(),
          std::move(config_values),
          configurator),
      tick_clock_(nullptr),
      is_captive_portal_(false),
      add_default_proxy_bypass_rules_(true) {}
