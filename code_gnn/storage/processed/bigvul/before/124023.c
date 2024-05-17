void BookmarkEventRouter::ExtensiveBookmarkChangesBeginning(
    BookmarkModel* model) {
  scoped_ptr<ListValue> args(new ListValue());
  DispatchEvent(model->profile(),
                keys::kOnBookmarkImportBegan,
                args.Pass());
}
