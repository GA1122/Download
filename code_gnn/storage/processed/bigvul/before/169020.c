void OfflinePageModelTaskified::OnDeleteDone(
    const DeletePageCallback& callback,
    DeletePageResult result,
    const std::vector<OfflinePageModel::DeletedPageInfo>& infos) {
  for (const auto& info : infos) {
    UMA_HISTOGRAM_ENUMERATION(
        "OfflinePages.DeletePageCount",
        model_utils::ToNamespaceEnum(info.client_id.name_space),
        OfflinePagesNamespaceEnumeration::RESULT_COUNT);
    for (Observer& observer : observers_)
      observer.OfflinePageDeleted(info);
  }
  InformDeletePageDone(callback, result);
}
