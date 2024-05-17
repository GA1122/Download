void NotCalled(mojo::ScopedSharedBufferHandle shared_buffer,
               mojo::ScopedHandle socket_handle,
               bool initially_muted) {
  EXPECT_TRUE(false) << "The StreamCreated callback was called despite the "
                        "test expecting it not to.";
}
