RenderThreadImpl::RequestCopyOfOutputForLayoutTest(
    int32_t routing_id,
    std::unique_ptr<viz::CopyOutputRequest> request) {
  DCHECK(layout_test_deps_);
  return layout_test_deps_->RequestCopyOfOutput(routing_id, std::move(request));
}
