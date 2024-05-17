void AXLayoutObject::lineBreaks(Vector<int>& lineBreaks) const {
  if (!isTextControl())
    return;

  VisiblePosition visiblePos = visiblePositionForIndex(0);
  VisiblePosition prevVisiblePos = visiblePos;
  visiblePos = nextLinePosition(visiblePos, LayoutUnit(), HasEditableAXRole);
  while (visiblePos.isNotNull() && !inSameLine(prevVisiblePos, visiblePos)) {
    lineBreaks.push_back(indexForVisiblePosition(visiblePos));
    prevVisiblePos = visiblePos;
    visiblePos = nextLinePosition(visiblePos, LayoutUnit(), HasEditableAXRole);

    if (visiblePos.deepEquivalent().compareTo(prevVisiblePos.deepEquivalent()) <
        0)
      break;
  }
}
