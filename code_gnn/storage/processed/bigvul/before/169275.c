bool NavigateIframeToURL(WebContents* web_contents,
                         std::string iframe_id,
                         const GURL& url) {
  std::string script = base::StringPrintf(
      "setTimeout(\""
      "var iframes = document.getElementById('%s');iframes.src='%s';"
      "\",0)",
      iframe_id.c_str(), url.spec().c_str());
  TestNavigationObserver load_observer(web_contents);
  bool result = ExecuteScript(web_contents, script);
  load_observer.Wait();
  return result;
}
