void PrintTo(const cc::PaintRecord& record, std::ostream* os) {
  Vector<cc::PaintOpType> ops;
  for (const auto* op : cc::PaintOpBuffer::Iterator(&record))
    ops.push_back(op->GetType());
  PrintTo(ops, os);
}
