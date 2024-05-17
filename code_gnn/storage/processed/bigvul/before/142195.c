    static bool MapStringToEntryType(base::StringPiece value, EntryType* type) {
      if (value == "file")
        *type = FILE;
      else if (value == "directory")
        *type = DIRECTORY;
      else if (value == "team_drive")
        *type = TEAM_DRIVE;
      else if (value == "Computer")
        *type = COMPUTER;
      else
        return false;
      return true;
    }
