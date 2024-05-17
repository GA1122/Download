void ServiceManagerConnectionImpl::SetConnectionLostClosure(
    const base::Closure& closure) {
  connection_lost_handler_ = closure;
}
