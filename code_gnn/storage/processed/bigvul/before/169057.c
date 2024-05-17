void OfflinePageModelImpl::GetPageByOfflineId(
    int64_t offline_id,
    const SingleOfflinePageItemCallback& callback) {
  std::vector<int64_t> query_ids;
  query_ids.emplace_back(offline_id);

  OfflinePageModelQueryBuilder builder;
  builder.SetOfflinePageIds(
      OfflinePageModelQuery::Requirement::INCLUDE_MATCHING, query_ids);

  auto multiple_callback = base::Bind(
      [](const SingleOfflinePageItemCallback& callback,
         const MultipleOfflinePageItemResult& result) {
        DCHECK_LE(result.size(), 1U);
        if (result.empty()) {
          callback.Run(nullptr);
        } else {
          callback.Run(&result[0]);
        }
      },
      callback);

  RunWhenLoaded(base::Bind(
      &OfflinePageModelImpl::GetPagesMatchingQueryWhenLoadDone,
      weak_ptr_factory_.GetWeakPtr(),
      base::Passed(builder.Build(GetPolicyController())), multiple_callback));
}
