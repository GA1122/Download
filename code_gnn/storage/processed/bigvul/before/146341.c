  void Parse() {
    while (HasMoreCharacters()) {
      Process(Current());
      if (HasMoreCharacters())
        Advance();
    }
  }
