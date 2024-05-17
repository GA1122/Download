  bool HasTabsPermission(const scoped_refptr<const Extension>& extension) {
    return HasTabsPermission(extension, tab_id());
  }
