void AXTree::SetEnableExtraMacNodes(bool enabled) {
  DCHECK(enable_extra_mac_nodes_ != enabled);
  DCHECK_EQ(0U, table_info_map_.size());
  enable_extra_mac_nodes_ = enabled;
}
