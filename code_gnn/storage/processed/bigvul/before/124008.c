void BookmarkEventRouter::BookmarkNodeAdded(BookmarkModel* model,
                                            const BookmarkNode* parent,
                                            int index) {
  scoped_ptr<ListValue> args(new ListValue());
  const BookmarkNode* node = parent->GetChild(index);
  args->Append(new StringValue(base::Int64ToString(node->id())));
  scoped_ptr<BookmarkTreeNode> tree_node(
      bookmark_api_helpers::GetBookmarkTreeNode(node, false, false));
  args->Append(tree_node->ToValue().release());

  DispatchEvent(model->profile(), keys::kOnBookmarkCreated, args.Pass());
}
