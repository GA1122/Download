  int GetWindowOrientationAngle() {
    int angle;
    ExecuteScriptAndGetValue(shell()->web_contents()->GetMainFrame(),
                             "window.orientation")->GetAsInteger(&angle);
    return angle;
  }
