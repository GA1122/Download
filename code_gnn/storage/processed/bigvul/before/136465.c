void PrintTo(const Vector<cc::PaintOpType>& ops, std::ostream* os) {
  *os << "[";
  bool first = true;
  for (auto op : ops) {
    if (first)
      first = false;
    else
      *os << ", ";
    *os << op;
  }
  *os << "]";
}
