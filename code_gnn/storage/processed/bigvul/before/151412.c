void FrameFetchContext::DispatchDidReceiveEncodedData(unsigned long identifier,
                                                      int encoded_data_length) {
  if (IsDetached())
    return;
  probe::didReceiveEncodedDataLength(GetFrame()->GetDocument(), identifier,
                                     encoded_data_length);
}
