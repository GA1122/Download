static void AssertWaitForStateOrDeleted(ClientStateNotification* note,
                                        EglRenderingVDAClient* client,
                                        ClientState expected_state) {
  ClientState state = note->Wait();
  if (state == expected_state) return;
  ASSERT_TRUE(client->decoder_deleted())
      << "Decoder not deleted but Wait() returned " << state
      << ", instead of " << expected_state;
}
