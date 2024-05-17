  void IterateOverPieces(const int sizes[],
                         Callback2<int, int>::Type* function) {
    DCHECK_GT(sizes[0], 0);

    int pos = 0;
    int index = 0;
    while (pos < kDataSize) {
      int size = std::min(sizes[index], kDataSize - pos);
      ++index;
      if (sizes[index] <= 0)
        index = 0;

      function->Run(pos, size);

      pos += size;
    }
    delete function;
  }
