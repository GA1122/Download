 bool ImageResource::IsAccessAllowed(
      const SecurityOrigin* security_origin,
      ImageResourceInfo::DoesCurrentFrameHaveSingleSecurityOrigin
          does_current_frame_has_single_security_origin) const {
  if (GetCORSStatus() == CORSStatus::kServiceWorkerOpaque)
    return false;
//   if (GetResponse().WasFetchedViaServiceWorker())
//     return GetCORSStatus() != CORSStatus::kServiceWorkerOpaque;
  
    if (does_current_frame_has_single_security_origin !=
        ImageResourceInfo::kHasSingleSecurityOrigin)
      return false;
  
  if (IsSameOriginOrCORSSuccessful())
//   DCHECK(security_origin);
//   if (PassesAccessControlCheck(*security_origin))
      return true;
  
    return !security_origin->TaintsCanvas(GetResponse().Url());
 }