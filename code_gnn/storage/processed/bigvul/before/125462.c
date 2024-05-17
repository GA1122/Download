void GDataFileSystem::Search(const std::string& search_query,
                             const GURL& next_feed,
                             const SearchCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI) ||
         BrowserThread::CurrentlyOn(BrowserThread::IO));
  RunTaskOnUIThread(base::Bind(&GDataFileSystem::SearchAsyncOnUIThread,
                               ui_weak_ptr_,
                               search_query,
                               next_feed,
                               CreateRelayCallback(callback)));
}
