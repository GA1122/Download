  void VerifyChromeProxyRequestHeader(
      const std::string& chrome_proxy_header_value) const {
    bool exp_found = false;
    for (const auto& attributes : base::SplitStringPiece(
             chrome_proxy_header_value, ",", base::TRIM_WHITESPACE,
             base::SPLIT_WANT_NONEMPTY)) {
      if (base::StartsWith(attributes,
                           "exp=", base::CompareCase::INSENSITIVE_ASCII)) {
        const auto attribute_split = base::SplitStringPiece(
            attributes, "=", base::TRIM_WHITESPACE, base::SPLIT_WANT_NONEMPTY);
        EXPECT_EQ(2u, attribute_split.size());
        EXPECT_EQ(expect_exp_value_in_request_header_, attribute_split[1]);
        exp_found = true;
      }
    }
    EXPECT_EQ(!expect_exp_value_in_request_header_.empty(), exp_found)
        << " expect_exp_value_in_request_header_="
        << expect_exp_value_in_request_header_;
  }
