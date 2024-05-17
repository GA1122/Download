void AudioOutputDeviceTest::ExpectRenderCallback(bool synchronized_io) {
  const int kMemorySize = CalculateMemorySize(synchronized_io);
  EXPECT_CALL(audio_output_ipc_, PlayStream(kStreamId))
      .WillOnce(SendPendingBytes(&browser_socket_, kMemorySize));

  if (synchronized_io) {
    EXPECT_CALL(callback_, RenderIO(_, _, _))
        .WillOnce(QuitLoop(io_loop_.message_loop_proxy()));
  } else {
    const int kNumberOfFramesToProcess = 0;
    EXPECT_CALL(callback_, Render(_, _))
        .WillOnce(DoAll(
            QuitLoop(io_loop_.message_loop_proxy()),
            Return(kNumberOfFramesToProcess)));
  }
}
