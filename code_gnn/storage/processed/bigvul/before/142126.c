  void AddTestData() {
    const BookmarkNode* bb_node = model_->bookmark_bar_node();
    std::string test_base = base_path();
    model_->AddURL(bb_node, 0, ASCIIToUTF16("a"), GURL(test_base + "a"));
    const BookmarkNode* f1 = model_->AddFolder(bb_node, 1, ASCIIToUTF16("F1"));
    model_->AddURL(f1, 0, ASCIIToUTF16("f1a"), GURL(test_base + "f1a"));
    const BookmarkNode* f11 = model_->AddFolder(f1, 1, ASCIIToUTF16("F11"));
    model_->AddURL(f11, 0, ASCIIToUTF16("f11a"), GURL(test_base + "f11a"));
    model_->AddFolder(bb_node, 2, ASCIIToUTF16("F2"));

    model_->AddURL(model_->other_node(), 0, ASCIIToUTF16("oa"),
                   GURL(test_base + "oa"));
    const BookmarkNode* of1 =
        model_->AddFolder(model_->other_node(), 1, ASCIIToUTF16("OF1"));
    model_->AddURL(of1, 0, ASCIIToUTF16("of1a"), GURL(test_base + "of1a"));
  }
