base::string16 GetHumanReadableRealm(const std::string& signon_realm) {
  FacetURI maybe_facet_uri(FacetURI::FromPotentiallyInvalidSpec(signon_realm));
  if (maybe_facet_uri.IsValidAndroidFacetURI())
    return base::UTF8ToUTF16("android://" +
                             maybe_facet_uri.android_package_name() + "/");
  return base::UTF8ToUTF16(signon_realm);
}
