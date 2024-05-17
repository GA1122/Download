void OfflinePageModelImpl::DeleteCachedPagesByURLPredicate(
    const UrlPredicate& predicate,
    const DeletePageCallback& callback) {
  RunWhenLoaded(
      base::Bind(&OfflinePageModelImpl::DoDeleteCachedPagesByURLPredicate,
                 weak_ptr_factory_.GetWeakPtr(), predicate, callback));
}
