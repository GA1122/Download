void TreeView::Cleanup() {
  RemoveObserverFromModel();

  STLDeleteContainerPairSecondPointers(id_to_details_map_.begin(),
                                       id_to_details_map_.end());
  id_to_details_map_.clear();
  node_to_details_map_.clear();

  if (image_list_) {
    ImageList_Destroy(image_list_);
    image_list_ = NULL;
  }
}
