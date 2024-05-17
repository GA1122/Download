  TestChunkerDisplayItem(const DisplayItemClient& client,
                         DisplayItem::Type type = DisplayItem::kDrawingFirst)
      : DisplayItem(client, type, sizeof(*this)) {}
