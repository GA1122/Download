  bool ScreenOrientationSupported() {
    bool support;
    ExecuteScriptAndGetValue(shell()->web_contents()->GetMainFrame(),
                             "'orientation' in screen")->GetAsBoolean(&support);
    return support;
  }
