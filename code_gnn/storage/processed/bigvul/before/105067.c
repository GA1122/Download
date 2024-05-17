void LiveSyncTest::SetUpTestServerIfRequired() {
  if (server_type_ == LOCAL_PYTHON_SERVER) {
    if (!SetUpLocalPythonTestServer())
      LOG(FATAL) << "Failed to set up local python test server";
  } else if (server_type_ == LOCAL_LIVE_SERVER) {
    if (!SetUpLocalTestServer())
      LOG(FATAL) << "Failed to set up local test server";
  } else if (server_type_ == EXTERNAL_LIVE_SERVER) {
  } else {
    LOG(FATAL) << "Don't know which server environment to run test in.";
  }
}
