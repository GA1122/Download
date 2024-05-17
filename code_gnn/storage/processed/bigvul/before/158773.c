  void CheckResourceLoaded(
      const GURL& url,
      const GURL& referrer,
      const std::string& load_method,
      content::ResourceType resource_type,
      const base::FilePath::StringPieceType& served_file_name,
      const std::string& mime_type,
      const std::string& ip_address,
      bool was_cached,
      bool first_network_request,
      const base::TimeTicks& before_request,
      const base::TimeTicks& after_request) {
    bool resource_load_info_found = false;
    for (const auto& resource_load_info : resource_load_infos_) {
      if (resource_load_info->url != url)
        continue;

      resource_load_info_found = true;
      int64_t file_size = -1;
      if (!served_file_name.empty()) {
        base::ScopedAllowBlockingForTesting allow_blocking;
        base::FilePath test_dir;
        ASSERT_TRUE(base::PathService::Get(content::DIR_TEST_DATA, &test_dir));
        base::FilePath served_file = test_dir.Append(served_file_name);
        ASSERT_TRUE(GetFileSize(served_file, &file_size));
      }
      EXPECT_EQ(referrer, resource_load_info->referrer);
      EXPECT_EQ(load_method, resource_load_info->method);
      EXPECT_EQ(resource_type, resource_load_info->resource_type);
      if (!first_network_request)
        EXPECT_GT(resource_load_info->request_id, 0);
      EXPECT_EQ(mime_type, resource_load_info->mime_type);
      ASSERT_TRUE(resource_load_info->network_info->ip_port_pair);
      EXPECT_EQ(ip_address,
                resource_load_info->network_info->ip_port_pair->host());
      EXPECT_EQ(was_cached, resource_load_info->was_cached);
      auto CheckTime = [before_request, after_request](auto actual) {
        EXPECT_LE(before_request, actual);
        EXPECT_GT(after_request, actual);
      };
      const net::LoadTimingInfo& timing = resource_load_info->load_timing_info;
      CheckTime(timing.request_start);
      CheckTime(timing.receive_headers_end);
      CheckTime(timing.send_start);
      CheckTime(timing.send_end);
      if (!was_cached) {
        CheckTime(timing.connect_timing.dns_start);
        CheckTime(timing.connect_timing.dns_end);
        CheckTime(timing.connect_timing.connect_start);
        CheckTime(timing.connect_timing.connect_end);
      }
      if (file_size != -1) {
        EXPECT_EQ(file_size, resource_load_info->raw_body_bytes);
        EXPECT_LT(file_size, resource_load_info->total_received_bytes);
      }
    }
    EXPECT_TRUE(resource_load_info_found);
  }
