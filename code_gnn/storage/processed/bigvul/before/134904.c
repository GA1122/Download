  void InitializeDatabase(crypto::ScopedPK11Slot system_slot) {
    crypto::ScopedPK11Slot system_slot_copy =
        crypto::ScopedPK11Slot(PK11_ReferenceSlot(system_slot.get()));
    auto database = std::make_unique<net::NSSCertDatabaseChromeOS>(
        std::move(system_slot)  ,
        crypto::ScopedPK11Slot()  );
    database->SetSystemSlot(std::move(system_slot_copy));
    system_token_cert_database_ = std::move(database);

    VLOG(1) << "SystemTokenCertDBInitializer: Passing system token NSS "
               "database to CertLoader.";
    CertLoader::Get()->SetSystemNSSDB(system_token_cert_database_.get());
  }
