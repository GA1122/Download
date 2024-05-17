static IMFSample* CreateEmptySample() {
  base::win::ScopedComPtr<IMFSample> sample;
  HRESULT hr = MFCreateSample(sample.Receive());
  RETURN_ON_HR_FAILURE(hr, "MFCreateSample failed", NULL);
  return sample.Detach();
}
