void OfflinePageModelImpl::GetAllPages(
    const MultipleOfflinePageItemCallback& callback) {
  OfflinePageModelQueryBuilder builder;
  RunWhenLoaded(
      base::Bind(&OfflinePageModelImpl::GetPagesMatchingQueryWhenLoadDone,
                 weak_ptr_factory_.GetWeakPtr(),
                 base::Passed(builder.Build(GetPolicyController())), callback));
}
