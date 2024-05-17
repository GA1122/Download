DataReductionProxyIOData::DataReductionProxyIOData(
    PrefService* prefs,
    scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner,
    scoped_refptr<base::SingleThreadTaskRunner> io_task_runner)
    : client_(Client::UNKNOWN),
      network_connection_tracker_(nullptr),
      io_task_runner_(io_task_runner),
      ui_task_runner_(ui_task_runner),
      effective_connection_type_(net::EFFECTIVE_CONNECTION_TYPE_UNKNOWN) {
  DCHECK(ui_task_runner_);
  DCHECK(io_task_runner_);
  network_properties_manager_.reset(new NetworkPropertiesManager(
      base::DefaultClock::GetInstance(), prefs, ui_task_runner_));
}
