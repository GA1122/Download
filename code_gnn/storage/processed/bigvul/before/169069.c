void OfflinePageModelImpl::MarkPageAccessed(int64_t offline_id) {
  RunWhenLoaded(base::Bind(&OfflinePageModelImpl::MarkPageAccessedWhenLoadDone,
                           weak_ptr_factory_.GetWeakPtr(), offline_id));
}
