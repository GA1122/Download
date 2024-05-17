  std::string ToLayerName(PaintedShape painted_shape) {
    switch (painted_shape) {
      case TOP_LEFT_CIRCLE:
        return "TOP_LEFT_CIRCLE";
      case TOP_RIGHT_CIRCLE:
        return "TOP_RIGHT_CIRCLE";
      case BOTTOM_RIGHT_CIRCLE:
        return "BOTTOM_RIGHT_CIRCLE";
      case BOTTOM_LEFT_CIRCLE:
        return "BOTTOM_LEFT_CIRCLE";
      case HORIZONTAL_RECT:
        return "HORIZONTAL_RECT";
      case VERTICAL_RECT:
        return "VERTICAL_RECT";
      case PAINTED_SHAPE_COUNT:
        NOTREACHED() << "The PAINTED_SHAPE_COUNT value should never be used.";
        return "PAINTED_SHAPE_COUNT";
    }
    return "UNKNOWN";
  }
