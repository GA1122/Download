GLuint GetServiceId(const OBJECT_TYPE* object) {
  return object ? object->service_id() : 0;
}
