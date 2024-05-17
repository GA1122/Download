void OptimizationHintsComponentInstallerPolicy::ComponentReady(
    const base::Version& version,
    const base::FilePath& install_dir,
    std::unique_ptr<base::DictionaryValue> manifest) {
  DCHECK(!install_dir.empty());
  DVLOG(1) << "Optimization Hints Version Ready: " << version.GetString();
  std::string ruleset_format;
  if (!manifest->GetString(kManifestRulesetFormatKey, &ruleset_format)) {
    DVLOG(1) << "No ruleset_format present in manifest";
    return;
  }
  base::Version ruleset_format_version = base::Version(ruleset_format);
  if (!ruleset_format_version.IsValid() ||
      ruleset_format_version.CompareTo(ruleset_format_version_) > 0) {
    DVLOG(1) << "Got incompatible ruleset_format. Bailing out.";
    return;
  }
  optimization_guide::OptimizationGuideService* optimization_guide_service =
      g_browser_process->optimization_guide_service();
  if (optimization_guide_service &&
      !base::CommandLine::ForCurrentProcess()->HasSwitch(
          kDisableInstallerUpdate)) {
    optimization_guide::HintsComponentInfo info(
        version,
        install_dir.Append(optimization_guide::kUnindexedHintsFileName));
    optimization_guide_service->MaybeUpdateHintsComponent(info);
  }
}
