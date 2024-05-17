void AXNodeObject::markers(Vector<DocumentMarker::MarkerType>& markerTypes,
                           Vector<AXRange>& markerRanges) const {
  if (!getNode() || !getDocument() || !getDocument()->view())
    return;

  DocumentMarkerController& markerController = getDocument()->markers();
  DocumentMarkerVector markers = markerController.markersFor(getNode());
  for (size_t i = 0; i < markers.size(); ++i) {
    DocumentMarker* marker = markers[i];
    switch (marker->type()) {
      case DocumentMarker::Spelling:
      case DocumentMarker::Grammar:
      case DocumentMarker::TextMatch:
        markerTypes.push_back(marker->type());
        markerRanges.push_back(
            AXRange(marker->startOffset(), marker->endOffset()));
        break;
      case DocumentMarker::Composition:
        break;
    }
  }
}
