void FrameFetchContext::DispatchDidDownloadToBlob(unsigned long identifier,
                                                  BlobDataHandle* blob) {
  if (IsDetached() || !blob)
    return;

  probe::didReceiveBlob(GetFrame()->GetDocument(), identifier,
                        MasterDocumentLoader(), blob);
}
