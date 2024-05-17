void OfflinePageModelImpl::DeletePagesByClientIds(
    const std::vector<ClientId>& client_ids,
    const DeletePageCallback& callback) {
  OfflinePageModelQueryBuilder builder;
  builder.SetClientIds(OfflinePageModelQuery::Requirement::INCLUDE_MATCHING,
                       client_ids);
  auto delete_pages = base::Bind(&OfflinePageModelImpl::DeletePages,
                                 weak_ptr_factory_.GetWeakPtr(), callback);
  RunWhenLoaded(base::Bind(
      &OfflinePageModelImpl::GetPagesMatchingQueryWhenLoadDone,
      weak_ptr_factory_.GetWeakPtr(),
      base::Passed(builder.Build(GetPolicyController())), delete_pages));
}
