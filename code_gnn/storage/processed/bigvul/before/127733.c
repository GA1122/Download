void TestTransactionConsumer::DidStart(int result) {
  if (result != OK) {
    DidFinish(result);
  } else {
    Read();
  }
}
