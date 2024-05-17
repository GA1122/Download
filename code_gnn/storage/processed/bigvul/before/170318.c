void UsbChooserContext::RevokeObjectPermission(
    const GURL& requesting_origin,
    const GURL& embedding_origin,
    const base::DictionaryValue& object) {
  std::string guid;
  if (object.GetString(kGuidKey, &guid)) {
    auto it = ephemeral_devices_.find(
        std::make_pair(requesting_origin, embedding_origin));
    if (it != ephemeral_devices_.end()) {
      it->second.erase(guid);
      if (it->second.empty())
        ephemeral_devices_.erase(it);
    }
    RecordPermissionRevocation(WEBUSB_PERMISSION_REVOKED_EPHEMERAL);
  } else {
    ChooserContextBase::RevokeObjectPermission(requesting_origin,
                                               embedding_origin, object);
    RecordPermissionRevocation(WEBUSB_PERMISSION_REVOKED);
  }
}
