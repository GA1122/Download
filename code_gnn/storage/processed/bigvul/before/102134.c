JsArgList GetNodeInfoById(const JsArgList& args,
                          UserShare* user_share,
                          DictionaryValue* (BaseNode::*info_getter)() const) {
  CHECK(info_getter);
  ListValue return_args;
  ListValue* node_summaries = new ListValue();
  return_args.Append(node_summaries);
  ListValue* id_list = NULL;
  ReadTransaction trans(FROM_HERE, user_share);
  if (args.Get().GetList(0, &id_list)) {
    CHECK(id_list);
    for (size_t i = 0; i < id_list->GetSize(); ++i) {
      int64 id = GetId(*id_list, i);
      if (id == kInvalidId) {
        continue;
      }
      ReadNode node(&trans);
      if (!node.InitByIdLookup(id)) {
        continue;
      }
      node_summaries->Append((node.*info_getter)());
    }
  }
  return JsArgList(&return_args);
}
