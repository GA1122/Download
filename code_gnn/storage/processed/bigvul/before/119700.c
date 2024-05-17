  bool JSExecuted(const std::string& script) {
    return content::ExecuteScript(web_contents(), script);
  }
