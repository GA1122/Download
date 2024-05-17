void RenderViewImpl::OnUpdatedFrameTree(
    int process_id,
    int route_id,
    const std::string& frame_tree) {
  DCHECK(false);
  if (!is_swapped_out_)
    return;

  base::DictionaryValue* frames = NULL;
  scoped_ptr<base::Value> tree(base::JSONReader::Read(frame_tree));
  if (tree.get() && tree->IsType(base::Value::TYPE_DICTIONARY))
    tree->GetAsDictionary(&frames);

  updating_frame_tree_ = true;
  active_frame_id_map_.clear();

  target_process_id_ = process_id;
  target_routing_id_ = route_id;
  CreateFrameTree(webview()->mainFrame(), frames);

  updating_frame_tree_ = false;
}
