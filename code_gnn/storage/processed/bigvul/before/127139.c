  void FillFormAndSubmit(const std::string& filename, const FormMap& data) {
    GURL url = test_server()->GetURL("files/autofill/" + filename);
    ui_test_utils::NavigateToURL(browser(), url);

    std::string js;
    for (FormMap::const_iterator i = data.begin(); i != data.end(); ++i) {
      js += "document.getElementById('" + i->first + "').value = '" +
            i->second + "';";
    }
    js += "document.getElementById('testform').submit();";

    WindowedPersonalDataManagerObserver observer(browser());
    ASSERT_TRUE(content::ExecuteScript(render_view_host(), js));
    observer.Wait();
  }
