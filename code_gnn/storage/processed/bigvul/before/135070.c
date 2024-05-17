 void AppCacheHost::GetStatusWithCallback(const GetStatusCallback& callback,
                                         void* callback_param) {
  DCHECK(pending_start_update_callback_.is_null() &&
         pending_swap_cache_callback_.is_null() &&
         pending_get_status_callback_.is_null());

  pending_get_status_callback_ = callback;
  pending_callback_param_ = callback_param;
  if (is_selection_pending())
    return;

  DoPendingGetStatus();
}
