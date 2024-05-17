  std::string ToLayerName(Dot dot) {
    switch (dot) {
      case BLUE_DOT:
        return "BLUE_DOT";
      case RED_DOT:
        return "RED_DOT";
      case YELLOW_DOT:
        return "YELLOW_DOT";
      case GREEN_DOT:
        return "GREEN_DOT";
      case DOT_COUNT:
        NOTREACHED() << "The DOT_COUNT value should never be used.";
        return "DOT_COUNT";
    }
     return "UNKNOWN";
   }
