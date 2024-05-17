void OfflinePageModelImpl::GetOfflineIdsForClientIdWhenLoadDone(
    const ClientId& client_id,
    const MultipleOfflineIdCallback& callback) const {
  DCHECK(is_loaded_);
  callback.Run(MaybeGetOfflineIdsForClientId(client_id));
}
