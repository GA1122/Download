void GDataFileSystem::SearchAsyncOnUIThread(
    const std::string& search_query,
    const GURL& next_feed,
    const SearchCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  scoped_ptr<std::vector<DocumentFeed*> > feed_list(
      new std::vector<DocumentFeed*>);

  ContentOrigin initial_origin = directory_service_->origin();
  feed_loader_->LoadFromServer(
      initial_origin,
      0, 0,   
      false,   
      FilePath(),   
      search_query,
      next_feed,
      std::string(),   
      FindEntryCallback(),   
      base::Bind(&GDataFileSystem::OnSearch, ui_weak_ptr_, callback));
}
