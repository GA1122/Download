  std::string GetManifestAndIssuedRequests() {
    base::RunLoop run_loop;
    browser()->tab_strip_model()->GetActiveWebContents()->GetManifest(
        base::Bind(&ManifestCallbackAndRun, run_loop.QuitClosure()));
    run_loop.Run();
    return ExecuteScriptAndExtractString(
        "if (issuedRequests.length != 0) reportRequests();"
        "else reportOnFetch = true;");
  }
