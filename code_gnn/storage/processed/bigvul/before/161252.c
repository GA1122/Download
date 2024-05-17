EmulationHandler::EmulationHandler()
    : DevToolsDomainHandler(Emulation::Metainfo::domainName),
      touch_emulation_enabled_(false),
      device_emulation_enabled_(false),
      host_(nullptr) {
}
