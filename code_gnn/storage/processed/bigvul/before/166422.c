void RecordContentDispositionCount(ContentDispositionCountTypes type,
                                   bool record) {
  if (!record)
    return;
  UMA_HISTOGRAM_ENUMERATION("Download.ContentDisposition", type,
                            CONTENT_DISPOSITION_LAST_ENTRY);
}
