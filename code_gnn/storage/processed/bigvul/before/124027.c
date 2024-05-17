bool BookmarksUpdateFunction::ExtractIds(const ListValue* args,
                                        std::list<int64>* ids,
                                        bool* invalid_id) {
  return BookmarksRemoveFunction::ExtractIds(args, ids, invalid_id);
}
