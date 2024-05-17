MockFramerVisitor::MockFramerVisitor() {
  ON_CALL(*this, OnPacketHeader(testing::_))
      .WillByDefault(testing::Return(true));
}
