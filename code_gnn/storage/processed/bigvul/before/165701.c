  InstallStaticUtilTest()
      : system_level_(std::string(std::get<1>(GetParam())) != "user"),
        scoped_install_details_(system_level_, std::get<0>(GetParam())),
        mode_(&InstallDetails::Get().mode()),
        root_key_(system_level_ ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER),
        nt_root_key_(system_level_ ? nt::HKLM : nt::HKCU) {}
