ACTION_P2(RunCallback, region, data) {
  SkRegion& dirty_region = data->mutable_dirty_region();
  dirty_region.op(region, SkRegion::kUnion_Op);
  arg0.Run(data);
}
