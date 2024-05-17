void VerifyBytesSentIsMultipleOf(uint64_t bytes) {
  EXPECT_EQ(0u, NetworkActivityMonitor::GetInstance()->GetBytesSent() % bytes);
}
