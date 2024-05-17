void AppCacheHost::DoPendingGetStatus() {
  DCHECK_EQ(false, pending_get_status_callback_.is_null());

  pending_get_status_callback_.Run(GetStatus(), pending_callback_param_);
  pending_get_status_callback_.Reset();
  pending_callback_param_ = NULL;
}
