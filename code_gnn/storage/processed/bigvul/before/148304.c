base::Closure WebContentsImpl::AddBindingSet(
    const std::string& interface_name,
    WebContentsBindingSet* binding_set) {
  auto result =
      binding_sets_.insert(std::make_pair(interface_name, binding_set));
  DCHECK(result.second);
  return base::Bind(&WebContentsImpl::RemoveBindingSet,
                    weak_factory_.GetWeakPtr(), interface_name);
}
