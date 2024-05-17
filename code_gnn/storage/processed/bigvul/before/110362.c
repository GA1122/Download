void AddRefResource(PP_Resource resource) {
  PpapiGlobals::Get()->GetResourceTracker()->AddRefResource(resource);
}
