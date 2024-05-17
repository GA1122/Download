LRESULT TreeView::OnNotify(int w_param, LPNMHDR l_param) {
  switch (l_param->code) {
    case TVN_GETDISPINFO: {
      DCHECK(model_);
      NMTVDISPINFO* info = reinterpret_cast<NMTVDISPINFO*>(l_param);

      if (!id_to_details_map_.empty()) {
        const NodeDetails* details =
            GetNodeDetailsByID(static_cast<int>(info->item.lParam));
        if (info->item.mask & TVIF_CHILDREN)
          info->item.cChildren = model_->GetChildCount(details->node);
        if (info->item.mask & TVIF_TEXT) {
          std::wstring text = details->node->GetTitle();
          DCHECK(info->item.cchTextMax);

          base::i18n::AdjustStringForLocaleDirection(&text);

          wcsncpy_s(info->item.pszText, info->item.cchTextMax, text.c_str(),
                    _TRUNCATE);
        }
        info->item.mask |= TVIF_DI_SETITEM;
      } else {
        if (info->item.mask & TVIF_CHILDREN)
          info->item.cChildren = 0;

        if (info->item.mask & TVIF_TEXT)
          wcsncpy_s(info->item.pszText, info->item.cchTextMax, L"", _TRUNCATE);
      }

      return 0;
    }

    case TVN_ITEMEXPANDING: {
      DCHECK(model_);
      NMTREEVIEW* info = reinterpret_cast<NMTREEVIEW*>(l_param);
      NodeDetails* details =
          GetNodeDetailsByID(static_cast<int>(info->itemNew.lParam));
      if (!details->loaded_children) {
        details->loaded_children = true;
        for (int i = 0; i < model_->GetChildCount(details->node); ++i) {
          CreateItem(details->tree_item, TVI_LAST,
                       model_->GetChild(details->node, i));
          if (auto_expand_children_)
            Expand(model_->GetChild(details->node, i));
        }
      }
      return FALSE;
    }

    case TVN_SELCHANGED:
      if (controller_)
        controller_->OnTreeViewSelectionChanged(this);
      break;

    case TVN_BEGINLABELEDIT: {
      NMTVDISPINFO* info = reinterpret_cast<NMTVDISPINFO*>(l_param);
      NodeDetails* details =
          GetNodeDetailsByID(static_cast<int>(info->item.lParam));
      if (!controller_ || controller_->CanEdit(this, details->node)) {
        editing_node_ = details->node;
        return FALSE;
      }
      return TRUE;
    }

    case TVN_ENDLABELEDIT: {
      NMTVDISPINFO* info = reinterpret_cast<NMTVDISPINFO*>(l_param);
      if (info->item.pszText) {
        NodeDetails* details =
            GetNodeDetailsByID(static_cast<int>(info->item.lParam));
        model_->SetTitle(details->node, info->item.pszText);
        editing_node_ = NULL;
        return FALSE;
      }
      editing_node_ = NULL;
      return 0;
    }

    case TVN_KEYDOWN:
      if (controller_) {
        NMTVKEYDOWN* key_down_message =
            reinterpret_cast<NMTVKEYDOWN*>(l_param);
        controller_->OnTreeViewKeyDown(
            ui::KeyboardCodeForWindowsKeyCode(key_down_message->wVKey));
      }
      break;

    default:
      break;
  }
  return 0;
}
