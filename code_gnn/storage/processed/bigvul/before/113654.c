void PPB_URLLoader_Impl::InstanceWasDeleted() {
  Resource::InstanceWasDeleted();
  loader_.reset();
}
