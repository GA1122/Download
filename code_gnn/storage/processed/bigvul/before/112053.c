    static CommitOrderingTest MakeLastCommitItem() {
      CommitOrderingTest last_commit_item;
      last_commit_item.commit_index = -1;
      last_commit_item.id = TestIdFactory::root();
      return last_commit_item;
    }
