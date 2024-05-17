  void LoadChangeFeed(const std::string& filename,
                      int largest_changestamp) {
    std::string error;
    scoped_ptr<Value> document(LoadJSONFile(filename));
    ASSERT_TRUE(document.get());
    ASSERT_TRUE(document->GetType() == Value::TYPE_DICTIONARY);
    scoped_ptr<DocumentFeed> document_feed(
        DocumentFeed::ExtractAndParse(*document));
    ASSERT_TRUE(document_feed.get());
    std::vector<DocumentFeed*> feed_list;
    feed_list.push_back(document_feed.get());
    ASSERT_TRUE(UpdateContent(feed_list, largest_changestamp));
  }
