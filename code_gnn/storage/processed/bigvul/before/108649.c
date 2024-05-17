  void VerifyReceivedDataMatchesChunks(URLRequest* r, TestDelegate* d) {
    const char* expected_data =
        "abcdthis is a longer chunk than before.\r\n\r\n02323";

    ASSERT_EQ(1, d->response_started_count()) << "request failed: " <<
        (int) r->status().status() << ", os error: " << r->status().error();

    EXPECT_FALSE(d->received_data_before_response());

    ASSERT_EQ(strlen(expected_data), static_cast<size_t>(d->bytes_received()));
    EXPECT_EQ(0, memcmp(d->data_received().c_str(), expected_data,
                        strlen(expected_data)));
  }
