void RemoveShaderInfo(int32_t id) {
  if (GetShaderCacheFactorySingleton())
    GetShaderCacheFactorySingleton()->RemoveCacheInfo(id);
}
