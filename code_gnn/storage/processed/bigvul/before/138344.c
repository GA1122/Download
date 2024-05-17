  void AddServiceManifest(base::StringPiece name, int resource_id) {
    std::string contents =
        GetContentClient()
            ->GetDataResource(resource_id, ui::ScaleFactor::SCALE_FACTOR_NONE)
            .as_string();
    DCHECK(!contents.empty());

    std::unique_ptr<base::Value> manifest_value =
        base::JSONReader::Read(contents);
    DCHECK(manifest_value);

    std::unique_ptr<base::Value> overlay_value =
        GetContentClient()->browser()->GetServiceManifestOverlay(name);

    service_manager::MergeManifestWithOverlay(manifest_value.get(),
                                              overlay_value.get());

    base::Optional<catalog::RequiredFileMap> required_files =
        catalog::RetrieveRequiredFiles(*manifest_value);
    if (required_files) {
      ChildProcessLauncher::SetRegisteredFilesForService(
          name.as_string(), std::move(*required_files));
    }

    auto result = manifests_.insert(
        std::make_pair(name.as_string(), std::move(manifest_value)));
    DCHECK(result.second) << "Duplicate manifest entry: " << name;
  }
