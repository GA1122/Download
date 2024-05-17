void CheckGetNodesByIdReturnArgs(const SyncManager& sync_manager,
                                 const JsArgList& return_args,
                                 int64 id,
                                 bool is_detailed) {
  EXPECT_EQ(1u, return_args.Get().GetSize());
  ListValue* nodes = NULL;
  ASSERT_TRUE(return_args.Get().GetList(0, &nodes));
  ASSERT_TRUE(nodes);
  EXPECT_EQ(1u, nodes->GetSize());
  DictionaryValue* node_info = NULL;
  EXPECT_TRUE(nodes->GetDictionary(0, &node_info));
  ASSERT_TRUE(node_info);
  ReadTransaction trans(FROM_HERE, sync_manager.GetUserShare());
  ReadNode node(&trans);
  EXPECT_TRUE(node.InitByIdLookup(id));
  CheckNodeValue(node, *node_info, is_detailed);
}
