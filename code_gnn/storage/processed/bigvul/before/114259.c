void GpuChannelHost::MessageFilter::RemoveRoute(int route_id) {
  DCHECK(parent_->factory_->IsIOThread());
  ListenerMap::iterator it = listeners_.find(route_id);
  if (it != listeners_.end())
    listeners_.erase(it);
}
