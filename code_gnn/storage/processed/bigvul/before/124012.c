void BookmarkEventRouter::BookmarkNodeMoved(BookmarkModel* model,
                                            const BookmarkNode* old_parent,
                                            int old_index,
                                            const BookmarkNode* new_parent,
                                            int new_index) {
  scoped_ptr<ListValue> args(new ListValue());
  const BookmarkNode* node = new_parent->GetChild(new_index);
  args->Append(new StringValue(base::Int64ToString(node->id())));
  DictionaryValue* object_args = new DictionaryValue();
  object_args->SetString(keys::kParentIdKey,
                         base::Int64ToString(new_parent->id()));
  object_args->SetInteger(keys::kIndexKey, new_index);
  object_args->SetString(keys::kOldParentIdKey,
                         base::Int64ToString(old_parent->id()));
  object_args->SetInteger(keys::kOldIndexKey, old_index);
  args->Append(object_args);

  DispatchEvent(model->profile(), keys::kOnBookmarkMoved, args.Pass());
}
