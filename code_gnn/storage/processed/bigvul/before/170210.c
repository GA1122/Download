  static bool FetchSubresource(content::WebContents* web_contents,
                               const GURL& url) {
    std::string script(
        "var xhr = new XMLHttpRequest();"
        "xhr.open('GET', '");
    script += url.spec() +
              "', true);"
              "xhr.onload = function (e) {"
              "  if (xhr.readyState === 4) {"
              "    window.domAutomationController.send(xhr.status === 200);"
              "  }"
              "};"
              "xhr.onerror = function () {"
              "  window.domAutomationController.send(false);"
              "};"
              "xhr.send(null)";
    bool xhr_result = false;
    bool execute_result =
        content::ExecuteScriptAndExtractBool(web_contents, script, &xhr_result);
    return xhr_result && execute_result;
  }
