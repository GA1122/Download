           media::VideoCaptureBufferType>::ToMojom(media::VideoCaptureBufferType
                                                       input) {
  switch (input) {
    case media::VideoCaptureBufferType::kSharedMemory:
      return media::mojom::VideoCaptureBufferType::kSharedMemory;
    case media::VideoCaptureBufferType::kSharedMemoryViaRawFileDescriptor:
      return media::mojom::VideoCaptureBufferType::
          kSharedMemoryViaRawFileDescriptor;
    case media::VideoCaptureBufferType::kMailboxHolder:
      return media::mojom::VideoCaptureBufferType::kMailboxHolder;
  }
  NOTREACHED();
  return media::mojom::VideoCaptureBufferType::kSharedMemory;
}
