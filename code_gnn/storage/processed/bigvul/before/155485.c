  void LoadOptimizationHints(const base::Version& ruleset_format) {
    std::unique_ptr<base::DictionaryValue> manifest(new base::DictionaryValue);
    if (ruleset_format.IsValid()) {
      manifest->SetString(
          OptimizationHintsComponentInstallerPolicy::kManifestRulesetFormatKey,
          ruleset_format.GetString());
    }
    ASSERT_TRUE(
        policy_->VerifyInstallation(*manifest, component_install_dir()));
    const base::Version expected_version(kTestHintsVersion);
    policy_->ComponentReady(expected_version, component_install_dir(),
                            std::move(manifest));
    base::RunLoop().RunUntilIdle();
  }
