void DataReductionProxyConfig::OnConnectionChanged(
    network::mojom::ConnectionType type) {
  DCHECK(thread_checker_.CalledOnValidThread());

  connection_type_ = type;
  RecordNetworkChangeEvent(NETWORK_CHANGED);

#if defined(OS_CHROMEOS)
  if (get_network_id_asynchronously_) {
    base::PostTaskAndReplyWithResult(
        g_get_network_id_task_runner.Get().get(), FROM_HERE,
        base::BindOnce(&DoGetCurrentNetworkID,
                       base::Unretained(network_connection_tracker_)),
        base::BindOnce(&DataReductionProxyConfig::ContinueNetworkChanged,
                       weak_factory_.GetWeakPtr()));
    return;
  }
#endif   

  ContinueNetworkChanged(GetCurrentNetworkID());
}
