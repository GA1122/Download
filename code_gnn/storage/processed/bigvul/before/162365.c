void NotCalled(mojo::ScopedSharedBufferHandle shared_buffer,
               mojo::ScopedHandle socket_handle) {
  EXPECT_TRUE(false) << "The StreamCreated callback was called despite the "
                        "test expecting it not to.";
}
