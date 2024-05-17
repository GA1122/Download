    MockQuotaManager()
        : QuotaManager(true  ,
                       base::FilePath(),
                       io_runner.get(),
                       nullptr,
                       storage::GetQuotaSettingsFunc()),
          async_(false) {}
