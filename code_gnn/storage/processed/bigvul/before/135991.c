  void TestGetSelectedTextReply(GURL url, bool expect_success) {
    ui_test_utils::NavigateToURL(browser(), url);
    content::WebContents* web_contents =
        browser()->tab_strip_model()->GetActiveWebContents();
    ASSERT_TRUE(pdf_extension_test_util::EnsurePDFHasLoaded(web_contents));

    content::BrowserPluginGuestManager* guest_manager =
        web_contents->GetBrowserContext()->GetGuestManager();
    content::WebContents* guest_contents = nullptr;
    ASSERT_NO_FATAL_FAILURE(guest_manager->ForEachGuest(
        web_contents, base::Bind(&GetGuestCallback, &guest_contents)));
    ASSERT_TRUE(guest_contents);
    ASSERT_TRUE(content::ExecuteScript(
        guest_contents,
        "var oldSendScriptingMessage = "
        "    PDFViewer.prototype.sendScriptingMessage_;"
        "PDFViewer.prototype.sendScriptingMessage_ = function(message) {"
        "  oldSendScriptingMessage.bind(this)(message);"
        "  if (message.type == 'getSelectedTextReply')"
        "    this.parentWindow_.postMessage('flush', '*');"
        "}"));

    bool success = false;
    ASSERT_TRUE(content::ExecuteScriptAndExtractBool(
        web_contents,
        "window.addEventListener('message', function(event) {"
        "  if (event.data == 'flush')"
        "    window.domAutomationController.send(false);"
        "  if (event.data.type == 'getSelectedTextReply')"
        "    window.domAutomationController.send(true);"
        "});"
        "document.getElementsByTagName('embed')[0].postMessage("
        "    {type: 'getSelectedText'});",
        &success));
    ASSERT_EQ(expect_success, success);
  }
