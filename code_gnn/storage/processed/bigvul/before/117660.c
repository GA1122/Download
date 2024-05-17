void RecordCmdLineAppHistogram() {
  UMA_HISTOGRAM_ENUMERATION(extension_misc::kAppLaunchHistogram,
                            extension_misc::APP_LAUNCH_CMD_LINE_APP,
                            extension_misc::APP_LAUNCH_BUCKET_BOUNDARY);
}
