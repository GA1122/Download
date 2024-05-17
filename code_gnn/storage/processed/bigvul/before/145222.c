void Dispatcher::InitOriginPermissions(const Extension* extension) {
  delegate_->InitOriginPermissions(extension,
                                   IsExtensionActive(extension->id()));
  UpdateOriginPermissions(
      extension->url(),
      URLPatternSet(),   
      extension->permissions_data()->GetEffectiveHostPermissions());
}
