  std::string escapeBarQuoted(std::string bar_quoted) {
    std::replace(bar_quoted.begin(), bar_quoted.end(), '|', '\"');
    return bar_quoted;
  }
