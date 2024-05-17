  int LayerIndex(const cc::Layer* layer) {
    for (size_t i = 0; i < RootLayer()->children().size(); ++i) {
      if (RootLayer()->children()[i] == layer)
        return i;
    }
    return -1;
   }
