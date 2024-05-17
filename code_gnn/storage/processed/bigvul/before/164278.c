  bool HasTabsPermission(const scoped_refptr<const Extension>& extension,
                         int tab_id) {
    return extension->permissions_data()->HasAPIPermissionForTab(
        tab_id, APIPermission::kTab);
  }
