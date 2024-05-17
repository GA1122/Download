void ResourceDispatcherHostImpl::OnInit() {
  scheduler_.reset(new ResourceScheduler);
}
