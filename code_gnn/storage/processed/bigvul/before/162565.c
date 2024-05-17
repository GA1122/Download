void Resource::RevalidationFailed() {
  SECURITY_CHECK(redirect_chain_.IsEmpty());
  ClearData();
  cache_handler_.Clear();
  integrity_disposition_ = ResourceIntegrityDisposition::kNotChecked;
  integrity_report_info_.Clear();
  DestroyDecodedDataForFailedRevalidation();
  is_revalidating_ = false;
}
