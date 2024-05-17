  PageInfo* page_info() {
    if (!page_info_.get()) {
      page_info_.reset(new PageInfo(
          mock_ui(), profile(), tab_specific_content_settings(), web_contents(),
          url(), security_level(), visible_security_state()));
    }
    return page_info_.get();
  }
