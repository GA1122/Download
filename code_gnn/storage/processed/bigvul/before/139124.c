void CacheShaderInfo(int32_t id, base::FilePath path) {
  if (GetShaderCacheFactorySingleton())
    GetShaderCacheFactorySingleton()->SetCacheInfo(id, path);
}
