  void ScheduleNavigatorStoreCredentialAtUnload(
      content::WebContents* web_contents,
      const char* username,
      const char* password) {
    ASSERT_TRUE(content::ExecuteScript(
        web_contents,
        base::StringPrintf(
            "window.addEventListener(\"unload\", () => {"
            "  var c = new PasswordCredential({ id: '%s', password: '%s' });"
            "  navigator.credentials.store(c);"
            "});",
            username, password)));
  }
