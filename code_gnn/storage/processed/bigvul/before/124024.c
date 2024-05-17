void BookmarkEventRouter::ExtensiveBookmarkChangesEnded(BookmarkModel* model) {
  scoped_ptr<ListValue> args(new ListValue());
  DispatchEvent(model->profile(),
                keys::kOnBookmarkImportEnded,
                args.Pass());
}
