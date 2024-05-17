  static bool ViewTypeMatches(ViewType::Type type, ViewType::Type match) {
    if (type == match)
      return true;

    if (match == ViewType::INVALID)
      return true;

    return false;
  }
