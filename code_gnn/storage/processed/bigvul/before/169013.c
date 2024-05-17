void OfflinePageModelTaskified::InformSavePageDone(
    const SavePageCallback& callback,
    SavePageResult result,
    const OfflinePageItem& page) {
  UMA_HISTOGRAM_ENUMERATION(
      "OfflinePages.SavePageCount",
      model_utils::ToNamespaceEnum(page.client_id.name_space),
      OfflinePagesNamespaceEnumeration::RESULT_COUNT);
  if (result == SavePageResult::ARCHIVE_CREATION_FAILED)
    CreateArchivesDirectoryIfNeeded();
  if (!callback.is_null())
    callback.Run(result, page.offline_id);
}
