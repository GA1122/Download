void TreeView::CreateItem(HTREEITEM parent_item,
                          HTREEITEM after,
                          TreeModelNode* node) {
  DCHECK(node);
  TVINSERTSTRUCT insert_struct = {0};
  insert_struct.hParent = parent_item;
  insert_struct.hInsertAfter = after;
  insert_struct.itemex.mask = TVIF_PARAM | TVIF_CHILDREN | TVIF_TEXT |
                              TVIF_SELECTEDIMAGE | TVIF_IMAGE;
  insert_struct.itemex.pszText = LPSTR_TEXTCALLBACK;
  insert_struct.itemex.cChildren = I_CHILDRENCALLBACK;
  int icon_index = model_->GetIconIndex(node);
  if (icon_index == -1) {
    insert_struct.itemex.iImage = 0;
    insert_struct.itemex.iSelectedImage = 1;
  } else {
    insert_struct.itemex.iImage = icon_index + 2;
    insert_struct.itemex.iSelectedImage = icon_index + 2;
  }
  int node_id = next_id_++;
  insert_struct.itemex.lParam = node_id;

  NodeDetails* node_details = new NodeDetails(node_id, node);

  DCHECK(node_to_details_map_.count(node) == 0);
  DCHECK(id_to_details_map_.count(node_id) == 0);

  node_to_details_map_[node] = node_details;
  id_to_details_map_[node_id] = node_details;

  node_details->tree_item = TreeView_InsertItem(tree_view_, &insert_struct);
}
