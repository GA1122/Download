  std::string GetReport() {
    EXPECT_TRUE(reports_.size() == 1);
    return reports_[0];
  }
