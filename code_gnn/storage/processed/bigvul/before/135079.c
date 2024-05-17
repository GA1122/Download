void AppCacheHost::OnServiceReinitialized(
    AppCacheStorageReference* old_storage_ref) {
  if (old_storage_ref->storage() == storage())
    disabled_storage_reference_ = old_storage_ref;
}
