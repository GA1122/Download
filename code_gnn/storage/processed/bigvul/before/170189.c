void CheckURLIsBlockedInWebContents(content::WebContents* web_contents,
                                    const GURL& url) {
  EXPECT_EQ(url, web_contents->GetURL());

  base::string16 blocked_page_title;
  if (url.has_host()) {
    blocked_page_title = base::UTF8ToUTF16(url.host());
  } else {
    blocked_page_title = base::UTF8ToUTF16(url.spec());
  }
  EXPECT_EQ(blocked_page_title, web_contents->GetTitle());

  bool result = false;
  EXPECT_TRUE(content::ExecuteScriptAndExtractBool(
      web_contents,
      "var textContent = document.body.textContent;"
      "var hasError = textContent.indexOf('ERR_BLOCKED_BY_ADMINISTRATOR') >= 0;"
      "domAutomationController.send(hasError);",
      &result));
  EXPECT_TRUE(result);
}
