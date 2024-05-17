void FileBrowserPrivateSearchDriveFunction::OnSearch(
    drive::FileError error,
    const GURL& next_link,
    scoped_ptr<SearchResultInfoList> results) {
  if (error != drive::FILE_ERROR_OK) {
    SendResponse(false);
    return;
  }

  DCHECK(results.get());
  const SearchResultInfoList& results_ref = *results.get();

  ConvertSearchResultInfoListToEntryDefinitionList(
      GetProfile(),
      extension_->id(),
      results_ref,
      base::Bind(&FileBrowserPrivateSearchDriveFunction::OnEntryDefinitionList,
                 this,
                 next_link,
                 base::Passed(&results)));
}
