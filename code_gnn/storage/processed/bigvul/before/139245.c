  void OnAudioParams(const AudioParameters& expected,
                     const AudioParameters& received) {
    EXPECT_TRUE(thread_checker_.CalledOnValidThread());
    EXPECT_EQ(expected.AsHumanReadableString(),
              received.AsHumanReadableString());
     AudioParametersReceived();
   }
