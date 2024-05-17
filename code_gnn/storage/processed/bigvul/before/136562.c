  void ExpectUnchangedState() {
    EXPECT_FALSE(root->Changed(*root));
    EXPECT_FALSE(node->Changed(*root));
    EXPECT_FALSE(child1->Changed(*root));
    EXPECT_FALSE(child2->Changed(*root));
    EXPECT_FALSE(grandchild1->Changed(*root));
     EXPECT_FALSE(grandchild2->Changed(*root));
   }
