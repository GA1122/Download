ServiceType GetServiceID(ClientType client_id,
                         ClientServiceMap<ClientType, ServiceType>* id_map,
                         bool create_if_missing,
                         GenFunction gen_function) {
  ServiceType service_id = id_map->invalid_service_id();
  if (id_map->GetServiceID(client_id, &service_id)) {
    return service_id;
  }

  if (create_if_missing) {
    service_id = gen_function();
    id_map->SetIDMapping(client_id, service_id);
    return service_id;
  }

  return id_map->invalid_service_id();
}
