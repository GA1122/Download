void TreeView::SetModel(TreeModel* model) {
  if (model == model_)
    return;
  if (model_ && tree_view_)
    DeleteRootItems();

  RemoveObserverFromModel();

  model_ = model;
  if (tree_view_ && model_) {
    CreateRootItems();
    AddObserverToModel();
    HIMAGELIST last_image_list = image_list_;
    image_list_ = CreateImageList();
    TreeView_SetImageList(tree_view_, image_list_, TVSIL_NORMAL);
    if (last_image_list)
      ImageList_Destroy(last_image_list);
  }
}
