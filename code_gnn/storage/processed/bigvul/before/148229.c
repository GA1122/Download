 void BrowserPpapiHostImpl::AddInstanceObserver(PP_Instance instance,
                                               InstanceObserver* observer) {
  instance_map_[instance]->observer_list.AddObserver(observer);
}
