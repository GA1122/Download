bool BookmarksRemoveFunction::ExtractIds(const ListValue* args,
                                        std::list<int64>* ids,
                                        bool* invalid_id) {
  std::string id_string;
  if (!args->GetString(0, &id_string))
    return false;
  int64 id;
  if (base::StringToInt64(id_string, &id))
    ids->push_back(id);
  else
    *invalid_id = true;
  return true;
}
