  static IntRect ChunkRectToLayer(const FloatRect& rect,
                                  const IntPoint& chunk_offset_from_layer) {
    FloatRect r = rect;
    r.MoveBy(chunk_offset_from_layer);
    return EnclosingIntRect(r);
  }
