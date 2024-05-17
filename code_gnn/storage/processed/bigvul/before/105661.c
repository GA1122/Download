bool TreeView::OnKeyDown(ui::KeyboardCode virtual_key_code) {
  if (virtual_key_code == VK_F2) {
    if (!GetEditingNode()) {
      TreeModelNode* selected_node = GetSelectedNode();
      if (selected_node)
        StartEditing(selected_node);
    }
    return true;
  } else if (virtual_key_code == ui::VKEY_RETURN && !process_enter_) {
    Widget* widget = GetWidget();
    DCHECK(widget);
    Accelerator accelerator(Accelerator(virtual_key_code,
                                        base::win::IsShiftPressed(),
                                        base::win::IsCtrlPressed(),
                                        base::win::IsAltPressed()));
    GetFocusManager()->ProcessAccelerator(accelerator);
    return true;
  }
  return false;
}
