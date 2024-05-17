ChromeURLRequestContextGetter::CreateOffTheRecord(
    Profile* profile,
    const ProfileIOData* profile_io_data,
    content::ProtocolHandlerMap* protocol_handlers) {
  DCHECK(profile->IsOffTheRecord());
  return new ChromeURLRequestContextGetter(
      new FactoryForMain(profile_io_data, protocol_handlers));
}
