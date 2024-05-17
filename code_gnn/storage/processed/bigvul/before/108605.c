  void AddChunksToUpload(URLRequest* r) {
    r->AppendChunkToUpload("a", 1, false);
    r->AppendChunkToUpload("bcd", 3, false);
    r->AppendChunkToUpload("this is a longer chunk than before.", 35, false);
    r->AppendChunkToUpload("\r\n\r\n", 4, false);
    r->AppendChunkToUpload("0", 1, false);
    r->AppendChunkToUpload("2323", 4, true);
  }
