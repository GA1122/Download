void DestroyCoordinatorTLS(void* tls_object) {
  delete reinterpret_cast<mojom::CoordinatorPtr*>(tls_object);
};
