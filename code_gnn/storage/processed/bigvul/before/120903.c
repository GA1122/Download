ChromeURLRequestContextGetter::CreateOriginalForExtensions(
    Profile* profile, const ProfileIOData* profile_io_data) {
  DCHECK(!profile->IsOffTheRecord());
  return new ChromeURLRequestContextGetter(
      new FactoryForExtensions(profile_io_data));
}
