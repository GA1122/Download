DataReductionProxyConfig::DataReductionProxyConfig(
    scoped_refptr<base::SingleThreadTaskRunner> io_task_runner,
    scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner,
    network::NetworkConnectionTracker* network_connection_tracker,
    std::unique_ptr<DataReductionProxyConfigValues> config_values,
    DataReductionProxyConfigurator* configurator)
    : unreachable_(false),
      enabled_by_user_(false),
      config_values_(std::move(config_values)),
      io_task_runner_(io_task_runner),
      ui_task_runner_(ui_task_runner),
      network_connection_tracker_(network_connection_tracker),
      configurator_(configurator),
      connection_type_(network::mojom::ConnectionType::CONNECTION_UNKNOWN),
      network_properties_manager_(nullptr),
      weak_factory_(this) {
  DCHECK(io_task_runner_);
  DCHECK(network_connection_tracker_);
  DCHECK(configurator);

  thread_checker_.DetachFromThread();
}
