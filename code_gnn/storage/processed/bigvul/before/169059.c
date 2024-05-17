void OfflinePageModelImpl::GetPagesByNamespace(
    const std::string& name_space,
    const MultipleOfflinePageItemCallback& callback) {
  OfflinePageModelQueryBuilder builder;
  builder.RequireNamespace(name_space);
  RunWhenLoaded(
      base::Bind(&OfflinePageModelImpl::GetPagesMatchingQueryWhenLoadDone,
                 weak_ptr_factory_.GetWeakPtr(),
                 base::Passed(builder.Build(GetPolicyController())), callback));
}
