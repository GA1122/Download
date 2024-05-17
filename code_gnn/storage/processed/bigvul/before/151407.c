void FrameFetchContext::DispatchDidDownloadData(unsigned long identifier,
                                                int data_length,
                                                int encoded_data_length) {
  if (IsDetached())
    return;

  GetFrame()->Loader().Progress().IncrementProgress(identifier, data_length);
  probe::didReceiveData(GetFrame()->GetDocument(), identifier,
                        MasterDocumentLoader(), 0, data_length);
  probe::didReceiveEncodedDataLength(GetFrame()->GetDocument(), identifier,
                                     encoded_data_length);
}
