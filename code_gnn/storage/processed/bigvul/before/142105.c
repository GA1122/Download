bool NeedsToEnrollWithMdm() {
  base::string16 mdm_url = GetMdmUrl();
  return !mdm_url.empty() && !IsEnrolledWithGoogleMdm(mdm_url);
}
