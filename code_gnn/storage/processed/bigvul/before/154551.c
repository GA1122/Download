error::Error DeleteHelper(ClientType client_id,
                          ClientServiceMap<ClientType, ServiceType>* id_map,
                          DeleteFunction delete_function) {
  delete_function(id_map->GetServiceIDOrInvalid(client_id));
  id_map->RemoveClientID(client_id);
  return error::kNoError;
}
