 void Vp9Parser::ReadTiles(Vp9FrameHeader* fhdr) {
  int sb64_cols = (fhdr->width + 63) / 64;

  int min_log2_tile_cols = GetMinLog2TileCols(sb64_cols);
  int max_log2_tile_cols = GetMaxLog2TileCols(sb64_cols);

  int max_ones = max_log2_tile_cols - min_log2_tile_cols;
  fhdr->log2_tile_cols = min_log2_tile_cols;
  while (max_ones-- && reader_.ReadBool())
    fhdr->log2_tile_cols++;

  if (reader_.ReadBool())
     fhdr->log2_tile_rows = reader_.ReadLiteral(2) - 1;
 }
