std::string Compress(const std::string& data) {
  std::string compressed;
  const bool result = metrics::GzipCompress(data, &compressed);
  EXPECT_TRUE(result);
  return compressed;
}
