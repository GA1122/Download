void GetTouchCalibration(TouchCalibration* cal) {
  std::vector<std::string> parts;
  if (Tokenize(base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
                   switches::kTouchCalibration),
               ",", &parts) >= 4) {
    if (!base::StringToInt(parts[0], &cal->bezel_left))
      LOG(ERROR) << "Incorrect left border calibration value passed.";
    if (!base::StringToInt(parts[1], &cal->bezel_right))
      LOG(ERROR) << "Incorrect right border calibration value passed.";
    if (!base::StringToInt(parts[2], &cal->bezel_top))
      LOG(ERROR) << "Incorrect top border calibration value passed.";
    if (!base::StringToInt(parts[3], &cal->bezel_bottom))
      LOG(ERROR) << "Incorrect bottom border calibration value passed.";
  }
}
