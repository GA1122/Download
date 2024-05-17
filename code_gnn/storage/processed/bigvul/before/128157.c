void CastConfigDelegateChromeos::CastToReceiver(
    const std::string& receiver_id) {
  ExecuteJavaScript("backgroundSetup.launchDesktopMirroring('" + receiver_id +
                     "');");
 }
