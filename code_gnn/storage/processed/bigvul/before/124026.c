bool BookmarksMoveFunction::ExtractIds(const ListValue* args,
                                      std::list<int64>* ids,
                                      bool* invalid_id) {
  return BookmarksUpdateFunction::ExtractIds(args, ids, invalid_id);
}
