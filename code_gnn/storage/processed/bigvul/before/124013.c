void BookmarkEventRouter::BookmarkNodeRemoved(BookmarkModel* model,
                                              const BookmarkNode* parent,
                                              int index,
                                              const BookmarkNode* node) {
  scoped_ptr<ListValue> args(new ListValue());
  args->Append(new StringValue(base::Int64ToString(node->id())));
  DictionaryValue* object_args = new DictionaryValue();
  object_args->SetString(keys::kParentIdKey,
                         base::Int64ToString(parent->id()));
  object_args->SetInteger(keys::kIndexKey, index);
  args->Append(object_args);

  DispatchEvent(model->profile(), keys::kOnBookmarkRemoved, args.Pass());
}
