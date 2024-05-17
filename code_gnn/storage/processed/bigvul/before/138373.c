void ServiceManagerConnectionImpl::OnConnectionLost() {
  if (!connection_lost_handler_.is_null())
    connection_lost_handler_.Run();
}
