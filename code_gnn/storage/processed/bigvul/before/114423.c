static IMFSample* CreateEmptySampleWithBuffer(int buffer_length, int align) {
  CHECK_GT(buffer_length, 0);

  base::win::ScopedComPtr<IMFSample> sample;
  sample.Attach(CreateEmptySample());

  base::win::ScopedComPtr<IMFMediaBuffer> buffer;
  HRESULT hr = E_FAIL;
  if (align == 0) {
    hr = MFCreateMemoryBuffer(buffer_length, buffer.Receive());
  } else {
    hr = MFCreateAlignedMemoryBuffer(buffer_length,
                                     align - 1,
                                     buffer.Receive());
  }
  RETURN_ON_HR_FAILURE(hr, "Failed to create memory buffer for sample", NULL);

  hr = sample->AddBuffer(buffer);
  RETURN_ON_HR_FAILURE(hr, "Failed to add buffer to sample", NULL);

  return sample.Detach();
}
