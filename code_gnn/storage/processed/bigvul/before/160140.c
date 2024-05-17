InFlightBackendIO::InFlightBackendIO(
    BackendImpl* backend,
    const scoped_refptr<base::SingleThreadTaskRunner>& background_thread)
    : backend_(backend),
      background_thread_(background_thread),
      ptr_factory_(this) {
}
