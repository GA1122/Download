service_manager::InterfaceProvider* Document::GetInterfaceProvider() {
  if (!GetFrame())
    return nullptr;

  return &GetFrame()->GetInterfaceProvider();
}
