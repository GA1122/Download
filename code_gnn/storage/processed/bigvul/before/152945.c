    ToMojom(media::VideoCaptureFrameDropReason input) {
  switch (input) {
    case media::VideoCaptureFrameDropReason::kNone:
      return media::mojom::VideoCaptureFrameDropReason::kNone;
    case media::VideoCaptureFrameDropReason::kDeviceClientFrameHasInvalidFormat:
      return media::mojom::VideoCaptureFrameDropReason::
          kDeviceClientFrameHasInvalidFormat;
    case media::VideoCaptureFrameDropReason::
        kDeviceClientLibyuvConvertToI420Failed:
      return media::mojom::VideoCaptureFrameDropReason::
          kDeviceClientLibyuvConvertToI420Failed;
    case media::VideoCaptureFrameDropReason::kV4L2BufferErrorFlagWasSet:
      return media::mojom::VideoCaptureFrameDropReason::
          kV4L2BufferErrorFlagWasSet;
    case media::VideoCaptureFrameDropReason::kV4L2InvalidNumberOfBytesInBuffer:
      return media::mojom::VideoCaptureFrameDropReason::
          kV4L2InvalidNumberOfBytesInBuffer;
    case media::VideoCaptureFrameDropReason::kAndroidThrottling:
      return media::mojom::VideoCaptureFrameDropReason::kAndroidThrottling;
    case media::VideoCaptureFrameDropReason::kAndroidGetByteArrayElementsFailed:
      return media::mojom::VideoCaptureFrameDropReason::
          kAndroidGetByteArrayElementsFailed;
    case media::VideoCaptureFrameDropReason::kAndroidApi1UnexpectedDataLength:
      return media::mojom::VideoCaptureFrameDropReason::
          kAndroidApi1UnexpectedDataLength;
    case media::VideoCaptureFrameDropReason::kAndroidApi2AcquiredImageIsNull:
      return media::mojom::VideoCaptureFrameDropReason::
          kAndroidApi2AcquiredImageIsNull;
    case media::VideoCaptureFrameDropReason::
        kWinDirectShowUnexpectedSampleLength:
      return media::mojom::VideoCaptureFrameDropReason::
          kWinDirectShowUnexpectedSampleLength;
    case media::VideoCaptureFrameDropReason::
        kWinDirectShowFailedToGetMemoryPointerFromMediaSample:
      return media::mojom::VideoCaptureFrameDropReason::
          kWinDirectShowFailedToGetMemoryPointerFromMediaSample;
    case media::VideoCaptureFrameDropReason::
        kWinMediaFoundationReceivedSampleIsNull:
      return media::mojom::VideoCaptureFrameDropReason::
          kWinMediaFoundationReceivedSampleIsNull;
    case media::VideoCaptureFrameDropReason::
        kWinMediaFoundationLockingBufferDelieveredNullptr:
      return media::mojom::VideoCaptureFrameDropReason::
          kWinMediaFoundationLockingBufferDelieveredNullptr;
    case media::VideoCaptureFrameDropReason::
        kWinMediaFoundationGetBufferByIndexReturnedNull:
      return media::mojom::VideoCaptureFrameDropReason::
          kWinMediaFoundationGetBufferByIndexReturnedNull;
    case media::VideoCaptureFrameDropReason::kBufferPoolMaxBufferCountExceeded:
      return media::mojom::VideoCaptureFrameDropReason::
          kBufferPoolMaxBufferCountExceeded;
    case media::VideoCaptureFrameDropReason::kBufferPoolBufferAllocationFailed:
      return media::mojom::VideoCaptureFrameDropReason::
          kBufferPoolBufferAllocationFailed;
  }
  NOTREACHED();
  return media::mojom::VideoCaptureFrameDropReason::kNone;
}
