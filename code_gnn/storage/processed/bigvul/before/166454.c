void RecordSavePackageEvent(SavePackageEvent event) {
  UMA_HISTOGRAM_ENUMERATION("Download.SavePackage", event,
                            SAVE_PACKAGE_LAST_ENTRY);
}
