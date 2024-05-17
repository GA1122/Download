void OfflinePageModelImpl::AddPageWhenLoadDone(
    const OfflinePageItem& page,
    const AddPageCallback& callback) {
  store_->AddOfflinePage(
      page, base::Bind(&OfflinePageModelImpl::OnAddPageDone,
                       weak_ptr_factory_.GetWeakPtr(), page, callback));
}
