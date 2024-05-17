void BlinkTestRunner::ResetScreenOrientation() {
  MockScreenOrientationClient* mock_client =
      proxy()->GetScreenOrientationClientMock();
  mock_client->ResetData();
}
