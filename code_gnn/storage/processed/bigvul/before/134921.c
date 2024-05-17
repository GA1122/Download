ChromeBrowserMainPartsChromeos::~ChromeBrowserMainPartsChromeos() {
  BootTimesRecorder::Get()->AddLogoutTimeMarker("LogoutDone", false);
  BootTimesRecorder::Get()->WriteLogoutTimes();
}
