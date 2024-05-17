void NavigateToFeedAndValidate(net::TestServer* server,
                               const std::string& url,
                               Browser* browser,
                               bool sniff_xml_type,
                               const std::string& expected_feed_title,
                               const std::string& expected_item_title,
                               const std::string& expected_item_desc,
                               const std::string& expected_error) {
  if (sniff_xml_type) {
  }

  ExtensionService* service = browser->profile()->GetExtensionService();
  const Extension* extension = service->extensions()->back();
  std::string id = extension->id();

  ui_test_utils::NavigateToURL(browser, GetFeedUrl(server, url, true, id));

  TabContents* tab = browser->GetSelectedTabContents();
  ASSERT_TRUE(ValidatePageElement(tab,
                                  L"",
                                  jscript_feed_title,
                                  expected_feed_title));
  ASSERT_TRUE(ValidatePageElement(tab,
                                  L"//html/body/div/iframe[1]",
                                  jscript_anchor,
                                  expected_item_title));
  ASSERT_TRUE(ValidatePageElement(tab,
                                  L"//html/body/div/iframe[1]",
                                  jscript_desc,
                                  expected_item_desc));
  ASSERT_TRUE(ValidatePageElement(tab,
                                  L"//html/body/div/iframe[1]",
                                  jscript_error,
                                  expected_error));
}
