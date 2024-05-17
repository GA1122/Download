bool IsWebcamAvailableOnSystem(WebContents* web_contents) {
  std::string result;
  EXPECT_TRUE(content::ExecuteScriptAndExtractString(
      web_contents, kHasVideoInputDeviceOnSystem, &result));
  return result == kHasVideoInputDevice;
}
