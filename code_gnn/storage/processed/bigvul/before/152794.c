inline bool NeedsColumnBalancing(LayoutUnit block_size,
                                 const ComputedStyle& style) {
  return block_size == kIndefiniteSize ||
          style.GetColumnFill() == EColumnFill::kBalance;
 }
