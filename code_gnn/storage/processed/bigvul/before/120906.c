ChromeURLRequestContextGetter::CreateOriginalForMedia(
    Profile* profile, const ProfileIOData* profile_io_data) {
  DCHECK(!profile->IsOffTheRecord());
  return new ChromeURLRequestContextGetter(
      new FactoryForMedia(profile_io_data));
}
