  std::string ExecutePNACLUrlLoaderTest(const std::string& mode) {
    std::string result(ExecuteScriptAndExtractString(
        base::StringPrintf("reportOnFetch = false;"
                           "var iframe = document.createElement('iframe');"
                           "iframe.src='%s#%s';"
                           "document.body.appendChild(iframe);",
                           test_page_url_.c_str(), mode.c_str())));
    EXPECT_EQ(base::StringPrintf("OnOpen%s", mode.c_str()), result);
    return ExecuteScriptAndExtractString("reportRequests();");
  }
