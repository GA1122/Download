void ExecuteJavaScriptAndWaitForLoadStop(WebContents* web_contents,
                                         const std::string script) {
  TestNavigationObserver observer(web_contents);

  static_cast<WebContentsImpl*>(web_contents)
      ->GetMainFrame()
      ->ExecuteJavaScriptForTests(base::UTF8ToUTF16(script));

  observer.Wait();
}
