void WebContentsImpl::RemoveBindingSet(const std::string& interface_name) {
  auto it = binding_sets_.find(interface_name);
  if (it != binding_sets_.end())
    binding_sets_.erase(it);
}
