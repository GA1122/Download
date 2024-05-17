void ReleaseResource(PP_Resource resource) {
  PpapiGlobals::Get()->GetResourceTracker()->ReleaseResource(resource);
}
