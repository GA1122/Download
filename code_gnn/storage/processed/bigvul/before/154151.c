GLuint GetClientId(const MANAGER_TYPE* manager, const OBJECT_TYPE* object) {
  DCHECK(manager);
  GLuint client_id = 0;
  if (object) {
    manager->GetClientId(object->service_id(), &client_id);
  }
  return client_id;
}
