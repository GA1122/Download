void GpuChannel::AppendAllCommandBufferStubs(
    std::vector<GpuCommandBufferStubBase*>& stubs) {
  for (StubMap::Iterator<GpuCommandBufferStub> it(&stubs_);
      !it.IsAtEnd(); it.Advance()) {
    stubs.push_back(it.GetCurrentValue());
  }
}
