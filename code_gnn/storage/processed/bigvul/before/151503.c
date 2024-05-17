SecurityOrigin* WorkerFetchContext::GetSecurityOrigin() const {
  return global_scope_->GetSecurityOrigin();
}
