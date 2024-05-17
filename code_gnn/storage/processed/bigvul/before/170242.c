update_client::CrxComponent ComponentUpdaterPolicyTest::MakeCrxComponent(
    bool supports_group_policy_enable_component_updates) {
  class MockInstaller : public update_client::CrxInstaller {
   public:
    MockInstaller() {}

    void Install(const base::FilePath& unpack_path,
                 const std::string& public_key,
                 Callback callback) override {
      DoInstall(unpack_path, public_key, std::move(callback));
    }

    MOCK_METHOD1(OnUpdateError, void(int error));
    MOCK_METHOD3(DoInstall,
                 void(const base::FilePath& unpack_path,
                      const std::string& public_key,
                      const Callback& callback));
    MOCK_METHOD2(GetInstalledFile,
                 bool(const std::string& file, base::FilePath* installed_file));
    MOCK_METHOD0(Uninstall, bool());

   private:
    ~MockInstaller() override {}
  };

  static const uint8_t jebg_hash[] = {
      0x94, 0x16, 0x0b, 0x6d, 0x41, 0x75, 0xe9, 0xec, 0x8e, 0xd5, 0xfa,
      0x54, 0xb0, 0xd2, 0xdd, 0xa5, 0x6e, 0x05, 0x6b, 0xe8, 0x73, 0x47,
      0xf6, 0xc4, 0x11, 0x9f, 0xbc, 0xb3, 0x09, 0xb3, 0x5b, 0x40};

  update_client::CrxComponent crx_component;
  crx_component.pk_hash.assign(std::begin(jebg_hash), std::end(jebg_hash));
  crx_component.version = base::Version("0.9");
  crx_component.installer = scoped_refptr<MockInstaller>(new MockInstaller());
  crx_component.requires_network_encryption = true;
  crx_component.supports_group_policy_enable_component_updates =
      supports_group_policy_enable_component_updates;

  return crx_component;
}
