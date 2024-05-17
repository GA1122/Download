error::Error CreateHelper(ClientType client_id,
                          ClientServiceMap<ClientType, ServiceType>* id_map,
                          GenFunction create_function) {
  if (id_map->HasClientID(client_id)) {
    return error::kInvalidArguments;
  }
  ServiceType service_id = create_function();
  id_map->SetIDMapping(client_id, service_id);
  return error::kNoError;
}
