GLsync GetSyncServiceID(GLuint client_id, PassthroughResources* resources) {
  return reinterpret_cast<GLsync>(
      resources->sync_id_map.GetServiceIDOrInvalid(client_id));
}
