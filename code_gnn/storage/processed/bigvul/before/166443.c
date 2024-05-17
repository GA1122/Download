void RecordInProgressDBCount(InProgressDBCountTypes type) {
  UMA_HISTOGRAM_ENUMERATION("Download.InProgressDB.Counts", type);
}
