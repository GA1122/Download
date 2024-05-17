void GeometryMapper::SourceToDestinationRect(
    const TransformPaintPropertyNode* source_transform_node,
    const TransformPaintPropertyNode* destination_transform_node,
    FloatRect& mapping_rect) {
  bool success = false;
  const TransformationMatrix& source_to_destination =
      SourceToDestinationProjectionInternal(
          source_transform_node, destination_transform_node, success);
  mapping_rect =
      success ? source_to_destination.MapRect(mapping_rect) : FloatRect();
}
