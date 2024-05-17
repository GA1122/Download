void BookmarksFunction::Loaded(BookmarkModel* model, bool ids_reassigned) {
  model->RemoveObserver(this);
  Run();
  Release();   
}
