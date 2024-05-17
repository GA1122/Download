void NavigationRequest::HandleInterfaceDisconnection(
    mojom::NavigationClientAssociatedPtr* navigation_client,
    base::OnceClosure error_handler) {
  navigation_client->set_connection_error_handler(std::move(error_handler));
}
