void DataReductionProxyConfigServiceClient::OnConnectionChanged(
    network::mojom::ConnectionType type) {
  DCHECK(thread_checker_.CalledOnValidThread());

  if (type == network::mojom::ConnectionType::CONNECTION_NONE)
    return;

  GetBackoffEntry()->Reset();
  last_ip_address_change_ = base::TimeTicks::Now();
  failed_attempts_before_success_ = 0;
  RetrieveConfig();
}
