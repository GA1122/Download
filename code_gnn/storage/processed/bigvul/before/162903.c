base::ProcessId CoordinatorImpl::GetProcessIdForClientIdentity(
    service_manager::Identity identity) const {
  DCHECK(identity.IsValid());
  return process_map_->GetProcessId(identity);
}
