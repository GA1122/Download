bool Browser::SupportsLocationBar() const {
  if (is_type_tabbed())
    return true;

  if (!is_app())
    return !is_trusted_source();

  if (hosted_app_controller_)
    return true;

  return false;
}
