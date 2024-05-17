void RecordOpensOutstanding(int size) {
  UMA_HISTOGRAM_CUSTOM_COUNTS("Download.OpensOutstanding", size, 1  ,
                              (1 << 10)  , 64  );
}
