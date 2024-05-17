void MediaStreamManager::SetGenerateStreamCallbackForTesting(
    GenerateStreamTestCallback test_callback) {
  generate_stream_test_callback_ = std::move(test_callback);
}
