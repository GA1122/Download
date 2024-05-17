ChromeURLRequestContextGetter::CreateOriginalForIsolatedMedia(
    Profile* profile,
    ChromeURLRequestContextGetter* app_context,
    const ProfileIOData* profile_io_data,
    const StoragePartitionDescriptor& partition_descriptor) {
  DCHECK(!profile->IsOffTheRecord());
  return new ChromeURLRequestContextGetter(
      new FactoryForIsolatedMedia(
          profile_io_data, partition_descriptor, app_context));
}
