    FromMojom(media::mojom::VideoCaptureFrameDropReason input,
              media::VideoCaptureFrameDropReason* output) {
  switch (input) {
    case media::mojom::VideoCaptureFrameDropReason::kNone:
      *output = media::VideoCaptureFrameDropReason::kNone;
      return true;
    case media::mojom::VideoCaptureFrameDropReason::
        kDeviceClientFrameHasInvalidFormat:
      *output = media::VideoCaptureFrameDropReason::
          kDeviceClientFrameHasInvalidFormat;
      return true;
    case media::mojom::VideoCaptureFrameDropReason::
        kDeviceClientLibyuvConvertToI420Failed:
      *output = media::VideoCaptureFrameDropReason::
          kDeviceClientLibyuvConvertToI420Failed;
      return true;
    case media::mojom::VideoCaptureFrameDropReason::kV4L2BufferErrorFlagWasSet:
      *output = media::VideoCaptureFrameDropReason::kV4L2BufferErrorFlagWasSet;
      return true;
    case media::mojom::VideoCaptureFrameDropReason::
        kV4L2InvalidNumberOfBytesInBuffer:
      *output =
          media::VideoCaptureFrameDropReason::kV4L2InvalidNumberOfBytesInBuffer;
      return true;
    case media::mojom::VideoCaptureFrameDropReason::kAndroidThrottling:
      *output = media::VideoCaptureFrameDropReason::kAndroidThrottling;
      return true;
    case media::mojom::VideoCaptureFrameDropReason::
        kAndroidGetByteArrayElementsFailed:
      *output = media::VideoCaptureFrameDropReason::
          kAndroidGetByteArrayElementsFailed;
      return true;
    case media::mojom::VideoCaptureFrameDropReason::
        kAndroidApi1UnexpectedDataLength:
      *output =
          media::VideoCaptureFrameDropReason::kAndroidApi1UnexpectedDataLength;
      return true;
    case media::mojom::VideoCaptureFrameDropReason::
        kAndroidApi2AcquiredImageIsNull:
      *output =
          media::VideoCaptureFrameDropReason::kAndroidApi2AcquiredImageIsNull;
      return true;
    case media::mojom::VideoCaptureFrameDropReason::
        kWinDirectShowUnexpectedSampleLength:
      *output = media::VideoCaptureFrameDropReason::
          kWinDirectShowUnexpectedSampleLength;
      return true;
    case media::mojom::VideoCaptureFrameDropReason::
        kWinDirectShowFailedToGetMemoryPointerFromMediaSample:
      *output = media::VideoCaptureFrameDropReason::
          kWinDirectShowFailedToGetMemoryPointerFromMediaSample;
      return true;
    case media::mojom::VideoCaptureFrameDropReason::
        kWinMediaFoundationReceivedSampleIsNull:
      *output = media::VideoCaptureFrameDropReason::
          kWinMediaFoundationReceivedSampleIsNull;
      return true;
    case media::mojom::VideoCaptureFrameDropReason::
        kWinMediaFoundationLockingBufferDelieveredNullptr:
      *output = media::VideoCaptureFrameDropReason::
          kWinMediaFoundationLockingBufferDelieveredNullptr;
      return true;
    case media::mojom::VideoCaptureFrameDropReason::
        kWinMediaFoundationGetBufferByIndexReturnedNull:
      *output = media::VideoCaptureFrameDropReason::
          kWinMediaFoundationGetBufferByIndexReturnedNull;
      return true;
    case media::mojom::VideoCaptureFrameDropReason::
        kBufferPoolMaxBufferCountExceeded:
      *output =
          media::VideoCaptureFrameDropReason::kBufferPoolMaxBufferCountExceeded;
      return true;
    case media::mojom::VideoCaptureFrameDropReason::
        kBufferPoolBufferAllocationFailed:
      *output =
          media::VideoCaptureFrameDropReason::kBufferPoolBufferAllocationFailed;
      return true;
  }
  NOTREACHED();
  return false;
}
