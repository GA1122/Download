  MockMojoVideoCaptureHost() : released_buffer_count_(0) {
    ON_CALL(*this, GetDeviceSupportedFormatsMock(_, _, _))
        .WillByDefault(WithArgs<2>(Invoke(RunEmptyFormatsCallback)));
    ON_CALL(*this, GetDeviceFormatsInUseMock(_, _, _))
        .WillByDefault(WithArgs<2>(Invoke(RunEmptyFormatsCallback)));
    ON_CALL(*this, ReleaseBuffer(_, _, _))
        .WillByDefault(InvokeWithoutArgs(
            this, &MockMojoVideoCaptureHost::increase_released_buffer_count));
  }
