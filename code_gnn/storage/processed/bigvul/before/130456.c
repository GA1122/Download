void ReportVersionWithUma(const base::Version& version) {
  DCHECK(!version.components().empty());
  uint32_t minor_version = 0;
  if (version.components().size() > 1)
    minor_version = version.components()[version.components().size() - 2];
  else
    minor_version = version.components()[0];
  UMA_HISTOGRAM_SPARSE_SLOWLY("SoftwareReporter.MinorVersion", minor_version);

  DCHECK_LT(version.components()[0], 0x100U);
  uint32_t major_version = 0x1000000 * version.components()[0];
  if (version.components().size() >= 2) {
    DCHECK_LT(version.components()[1], 0x10000U);
    major_version += 0x100 * version.components()[1];
  }
  if (version.components().size() >= 3) {
    DCHECK_LT(version.components()[2], 0x100U);
    major_version += version.components()[2];
  }
  UMA_HISTOGRAM_SPARSE_SLOWLY("SoftwareReporter.MajorVersion", major_version);
}
