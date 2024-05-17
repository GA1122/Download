  FactoryForMain(
      const ProfileIOData* profile_io_data,
      content::ProtocolHandlerMap* protocol_handlers)
      : profile_io_data_(profile_io_data) {
    std::swap(protocol_handlers_, *protocol_handlers);
  }
