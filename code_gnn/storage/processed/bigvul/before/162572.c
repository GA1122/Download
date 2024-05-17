void Resource::SetDataBufferingPolicy(
    DataBufferingPolicy data_buffering_policy) {
  options_.data_buffering_policy = data_buffering_policy;
  ClearData();
  SetEncodedSize(0);
}
