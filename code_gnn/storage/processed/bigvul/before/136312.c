bool PaintPropertyTreeBuilder::ObjectIsRepeatingTableSectionInPagedMedia()
    const {
  if (!IsRepeatingTableSection(object_))
    return false;

  if (context_.painting_layer->EnclosingPaginationLayer())
    return false;

  if (!object_.View()->PageLogicalHeight())
    return false;

  if (!object_.View()->IsHorizontalWritingMode())
    return false;

  return true;
}
