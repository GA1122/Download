  void FillForm(std::string node_id, std::string value) {
    GURL url("javascript:fillForm('" + node_id + "', '" + value + "')");
    ASSERT_TRUE(tab_->NavigateToURLAsync(url));
  }
