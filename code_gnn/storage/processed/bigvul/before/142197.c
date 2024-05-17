    static bool MapStringToSharedOption(base::StringPiece value,
                                        SharedOption* option) {
      if (value == "shared")
        *option = SHARED;
      else if (value == "sharedWithMe")
        *option = SHARED_WITH_ME;
      else if (value == "nestedSharedWithMe")
        *option = NESTED_SHARED_WITH_ME;
      else if (value == "none")
        *option = NONE;
      else
        return false;
      return true;
    }
