int Center(int size, int item_size) {
  int extra_space = size - item_size;
  if (extra_space > 0)
    ++extra_space;
  return extra_space / 2;
}
