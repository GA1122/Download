    void TestPlaybackRate(double playback_rate) {
    static const int kDefaultBufferSize = kSamplesPerSecond / 10;
    static const int kDefaultFramesRequested = 5 * kSamplesPerSecond;
//     const int kDefaultBufferSize = algorithm_.samples_per_second() / 10;
//     const int kDefaultFramesRequested = 2 * algorithm_.samples_per_second();
  
      TestPlaybackRate(playback_rate, kDefaultBufferSize,
                       kDefaultFramesRequested);
   }