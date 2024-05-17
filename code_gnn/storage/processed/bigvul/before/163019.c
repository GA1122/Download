ProfilingProcessHost::GetMetadataJSONForTrace() {
  std::unique_ptr<base::DictionaryValue> metadata_dict(
      new base::DictionaryValue);
  metadata_dict->SetKey(
      "product-version",
      base::Value(version_info::GetProductNameAndVersionForUserAgent()));
  metadata_dict->SetKey("user-agent", base::Value(GetUserAgent()));
  metadata_dict->SetKey("os-name",
                        base::Value(base::SysInfo::OperatingSystemName()));
  metadata_dict->SetKey(
      "command_line",
      base::Value(
          base::CommandLine::ForCurrentProcess()->GetCommandLineString()));
  metadata_dict->SetKey(
      "os-arch", base::Value(base::SysInfo::OperatingSystemArchitecture()));
  return metadata_dict;
}
