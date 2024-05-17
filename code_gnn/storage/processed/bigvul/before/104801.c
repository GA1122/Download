void ExtensionService::GrantPermissions(const Extension* extension) {
  CHECK(extension);

  CHECK_EQ(Extension::INTERNAL, extension->location());

  URLPatternSet effective_hosts = extension->GetEffectiveHostPermissions();
  extension_prefs_->AddGrantedPermissions(extension->id(),
                                          extension->HasFullPermissions(),
                                          extension->api_permissions(),
                                          effective_hosts);
}
