  void AssertReportSent() {
    ui_test_utils::RunMessageLoop();

    FakeSafeBrowsingService* service =
        static_cast<FakeSafeBrowsingService*>(
            g_browser_process->safe_browsing_service());

    std::string serialized = service->GetReport();

    safe_browsing::ClientMalwareReportRequest report;
    ASSERT_TRUE(report.ParseFromString(serialized));

    EXPECT_TRUE(report.complete());
  }
