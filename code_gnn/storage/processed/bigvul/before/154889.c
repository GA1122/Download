error::Error GenHelper(GLsizei n,
                       const volatile ClientType* client_ids,
                       ClientServiceMap<ClientType, ServiceType>* id_map,
                       GenFunction gen_function) {
  DCHECK(n >= 0);
  std::vector<ClientType> client_ids_copy(client_ids, client_ids + n);
  for (GLsizei ii = 0; ii < n; ++ii) {
    if (id_map->HasClientID(client_ids_copy[ii])) {
      return error::kInvalidArguments;
    }
  }
  if (!CheckUniqueAndNonNullIds(n, client_ids_copy.data())) {
    return error::kInvalidArguments;
  }

  std::vector<ServiceType> service_ids(n, 0);
  gen_function(n, service_ids.data());
  for (GLsizei ii = 0; ii < n; ++ii) {
    id_map->SetIDMapping(client_ids_copy[ii], service_ids[ii]);
  }

  return error::kNoError;
}
