  void SubmitForm(std::string node_id) {
    GURL url("javascript:submitForm('" + node_id + "')");
    ASSERT_TRUE(tab_->NavigateToURL(url));
  }
