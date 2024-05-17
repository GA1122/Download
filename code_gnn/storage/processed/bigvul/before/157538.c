TestDataReductionProxyConfig::TestDataReductionProxyConfig(
    scoped_refptr<base::SingleThreadTaskRunner> io_task_runner,
    scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner,
    DataReductionProxyConfigurator* configurator)
    : TestDataReductionProxyConfig(
          std::make_unique<TestDataReductionProxyParams>(),
          io_task_runner,
          ui_task_runner,
          configurator) {}
