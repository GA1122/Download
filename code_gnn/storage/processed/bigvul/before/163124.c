void IncrementNumber(size_t* number, BlobStatus status) {
  EXPECT_EQ(BlobStatus::DONE, status);
  *number = *number + 1;
}
