  BookmarkNode* GetMutableNode(const std::string& name) {
    return const_cast<BookmarkNode*>(GetNode(name));
  }
