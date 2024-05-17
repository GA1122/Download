void OfflinePageModelImpl::GetOfflineIdsForClientId(
    const ClientId& client_id,
    const MultipleOfflineIdCallback& callback) {
  RunWhenLoaded(
      base::Bind(&OfflinePageModelImpl::GetOfflineIdsForClientIdWhenLoadDone,
                 weak_ptr_factory_.GetWeakPtr(), client_id, callback));
}
