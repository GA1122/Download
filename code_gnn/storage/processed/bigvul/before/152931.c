    FromMojom(media::mojom::VideoCaptureBufferType input,
              media::VideoCaptureBufferType* output) {
  switch (input) {
    case media::mojom::VideoCaptureBufferType::kSharedMemory:
      *output = media::VideoCaptureBufferType::kSharedMemory;
      return true;
    case media::mojom::VideoCaptureBufferType::
        kSharedMemoryViaRawFileDescriptor:
      *output =
          media::VideoCaptureBufferType::kSharedMemoryViaRawFileDescriptor;
      return true;
    case media::mojom::VideoCaptureBufferType::kMailboxHolder:
      *output = media::VideoCaptureBufferType::kMailboxHolder;
      return true;
  }
  NOTREACHED();
  return false;
}
