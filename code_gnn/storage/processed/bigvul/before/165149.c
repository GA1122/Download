bool HTMLFormElement::LayoutObjectIsNeeded(const ComputedStyle& style) {
  if (!was_demoted_)
    return HTMLElement::LayoutObjectIsNeeded(style);

  ContainerNode* node = parentNode();
  if (!node || !node->GetLayoutObject())
    return HTMLElement::LayoutObjectIsNeeded(style);
  LayoutObject* parent_layout_object = node->GetLayoutObject();
  bool parent_is_table_element_part =
      (parent_layout_object->IsTable() && IsHTMLTableElement(*node)) ||
      (parent_layout_object->IsTableRow() && IsHTMLTableRowElement(*node)) ||
      (parent_layout_object->IsTableSection() && node->HasTagName(tbodyTag)) ||
      (parent_layout_object->IsLayoutTableCol() && node->HasTagName(colTag)) ||
      (parent_layout_object->IsTableCell() && IsHTMLTableRowElement(*node));

  if (!parent_is_table_element_part)
    return true;

  EDisplay display = style.Display();
  bool form_is_table_part =
      display == EDisplay::kTable || display == EDisplay::kInlineTable ||
      display == EDisplay::kTableRowGroup ||
      display == EDisplay::kTableHeaderGroup ||
      display == EDisplay::kTableFooterGroup ||
      display == EDisplay::kTableRow ||
      display == EDisplay::kTableColumnGroup ||
      display == EDisplay::kTableColumn || display == EDisplay::kTableCell ||
      display == EDisplay::kTableCaption;

  return form_is_table_part;
}
