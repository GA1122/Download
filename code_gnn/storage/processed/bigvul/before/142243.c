  base::FilePath base_path() const {
    if (base::FeatureList::IsEnabled(chromeos::features::kMyFilesVolume))
      return root_path().Append("Downloads");

    return root_path();
  }
