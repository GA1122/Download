void AXTreeSerializerTest::CreateTreeSerializer() {
  if (serializer_)
    return;

  tree0_.reset(new AXSerializableTree(treedata0_));
  tree1_.reset(new AXSerializableTree(treedata1_));

  tree0_source_.reset(tree0_->CreateTreeSource());
  serializer_.reset(new AXTreeSerializer<const AXNode*>(tree0_source_.get()));
  AXTreeUpdate unused_update;
  serializer_->SerializeChanges(tree0_->root(), &unused_update);

  tree1_source_.reset(tree1_->CreateTreeSource());
  serializer_->ChangeTreeSourceForTesting(tree1_source_.get());
}
