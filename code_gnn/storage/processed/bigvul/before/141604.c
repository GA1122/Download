  void AddFilter(std::vector<Filter>* filters,
                 std::string filter,
                 Filter::Type type = Filter::ALLOW) {
    filters->push_back(Filter(base::ASCIIToUTF16(filter), type));
  }
