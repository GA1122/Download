int64 MakeNodeWithParent(UserShare* share,
                         ModelType model_type,
                         const std::string& client_tag,
                         int64 parent_id) {
  WriteTransaction trans(FROM_HERE, share);
  ReadNode parent_node(&trans);
  EXPECT_EQ(BaseNode::INIT_OK, parent_node.InitByIdLookup(parent_id));
  WriteNode node(&trans);
  sync_api::WriteNode::InitUniqueByCreationResult result =
      node.InitUniqueByCreation(model_type, parent_node, client_tag);
  EXPECT_EQ(sync_api::WriteNode::INIT_SUCCESS, result);
  node.SetIsFolder(false);
  return node.GetId();
}
