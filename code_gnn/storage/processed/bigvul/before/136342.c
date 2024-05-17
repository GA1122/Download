  ~FragmentPaintPropertyTreeBuilder() {
    full_context_.force_subtree_update |= property_added_or_removed_;
#if DCHECK_IS_ON()
    if (properties_)
      PaintPropertyTreePrinter::UpdateDebugNames(object_, *properties_);
#endif
  }
