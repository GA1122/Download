 AccessControlStatus ScriptResource::CalculateAccessControlStatus() const {
  if (GetCORSStatus() == CORSStatus::kServiceWorkerOpaque)
    return kOpaqueResource;
// AccessControlStatus ScriptResource::CalculateAccessControlStatus(
//     const SecurityOrigin* security_origin) const {
//   if (GetResponse().WasFetchedViaServiceWorker()) {
//     if (GetCORSStatus() == CORSStatus::kServiceWorkerOpaque)
//       return kOpaqueResource;
//     return kSharableCrossOrigin;
//   }
  
  if (IsSameOriginOrCORSSuccessful())
//   if (security_origin && PassesAccessControlCheck(*security_origin))
      return kSharableCrossOrigin;
  
    return kNotSharableCrossOrigin;
 }