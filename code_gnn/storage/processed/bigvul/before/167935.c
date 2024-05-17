service_manager::InterfaceProvider& LocalFrame::GetInterfaceProvider() {
  DCHECK(Client());
  return *Client()->GetInterfaceProvider();
}
