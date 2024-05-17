  void CheckSystemProfile(const SystemProfileProto& system_profile) {
    EXPECT_EQ(TestMetricsServiceClient::kBrandForTesting,
              system_profile.brand_code());

    const SystemProfileProto::Hardware& hardware =
        system_profile.hardware();

    EXPECT_TRUE(hardware.has_cpu());
    EXPECT_TRUE(hardware.cpu().has_vendor_name());
    EXPECT_TRUE(hardware.cpu().has_signature());
    EXPECT_TRUE(hardware.cpu().has_num_cores());

  }
