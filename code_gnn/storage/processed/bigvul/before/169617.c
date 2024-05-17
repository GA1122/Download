void TestURLLoader::RunTests(const std::string& filter) {
  RUN_CALLBACK_TEST(TestURLLoader, BasicGET, filter);
  RUN_CALLBACK_TEST(TestURLLoader, BasicPOST, filter);
  RUN_CALLBACK_TEST(TestURLLoader, BasicFilePOST, filter);
  RUN_CALLBACK_TEST(TestURLLoader, BasicFileRangePOST, filter);
  RUN_CALLBACK_TEST(TestURLLoader, CompoundBodyPOST, filter);
  RUN_CALLBACK_TEST(TestURLLoader, EmptyDataPOST, filter);
  RUN_CALLBACK_TEST(TestURLLoader, BinaryDataPOST, filter);
  RUN_CALLBACK_TEST(TestURLLoader, CustomRequestHeader, filter);
  RUN_CALLBACK_TEST(TestURLLoader, FailsBogusContentLength, filter);
  RUN_CALLBACK_TEST(TestURLLoader, StreamToFile, filter);
  RUN_CALLBACK_TEST(TestURLLoader, UntrustedJavascriptURLRestriction, filter);
  RUN_CALLBACK_TEST(TestURLLoader, TrustedJavascriptURLRestriction, filter);
  RUN_CALLBACK_TEST(TestURLLoader, UntrustedHttpRequests, filter);
  RUN_CALLBACK_TEST(TestURLLoader, TrustedHttpRequests, filter);
  RUN_CALLBACK_TEST(TestURLLoader, FollowURLRedirect, filter);
  RUN_CALLBACK_TEST(TestURLLoader, AuditURLRedirect, filter);
  RUN_CALLBACK_TEST(TestURLLoader, AbortCalls, filter);
  RUN_CALLBACK_TEST(TestURLLoader, UntendedLoad, filter);
  RUN_CALLBACK_TEST(TestURLLoader, PrefetchBufferThreshold, filter);
  RUN_CALLBACK_TEST(TestURLLoader, XRequestedWithHeader, filter);
  RUN_CALLBACK_TEST(TestURLLoader, UntrustedSameOriginRestriction, filter);
  RUN_CALLBACK_TEST(TestURLLoader, UntrustedCrossOriginRequest, filter);
  RUN_CALLBACK_TEST(TestURLLoader, UntrustedCorbEligibleRequest, filter);
  RUN_CALLBACK_TEST(TestURLLoader, TrustedSameOriginRestriction, filter);
  RUN_CALLBACK_TEST(TestURLLoader, TrustedCrossOriginRequest, filter);
  RUN_CALLBACK_TEST(TestURLLoader, TrustedCorbEligibleRequest, filter);
}
