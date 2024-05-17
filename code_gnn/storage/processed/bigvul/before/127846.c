  int GetOrientationAngle() {
    int angle;
    ExecuteScriptAndGetValue(shell()->web_contents()->GetMainFrame(),
                             "screen.orientation.angle")->GetAsInteger(&angle);
    return angle;
  }
