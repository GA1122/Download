  void ExpectInitialState() {
    EXPECT_FALSE(root->Changed(*root));
    EXPECT_TRUE(node->Changed(*root));
    EXPECT_TRUE(child1->Changed(*node));
    EXPECT_TRUE(child2->Changed(*node));
    EXPECT_TRUE(grandchild1->Changed(*child1));
    EXPECT_TRUE(grandchild2->Changed(*child2));
  }
