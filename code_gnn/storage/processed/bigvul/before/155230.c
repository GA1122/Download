bool HTMLFormElement::layoutObjectIsNeeded(const ComputedStyle& style) {
  if (!m_wasDemoted)
    return HTMLElement::layoutObjectIsNeeded(style);

  ContainerNode* node = parentNode();
  if (!node || !node->layoutObject())
    return HTMLElement::layoutObjectIsNeeded(style);
  LayoutObject* parentLayoutObject = node->layoutObject();
  bool parentIsTableElementPart =
      (parentLayoutObject->isTable() && isHTMLTableElement(*node)) ||
      (parentLayoutObject->isTableRow() && isHTMLTableRowElement(*node)) ||
      (parentLayoutObject->isTableSection() && node->hasTagName(tbodyTag)) ||
      (parentLayoutObject->isLayoutTableCol() && node->hasTagName(colTag)) ||
      (parentLayoutObject->isTableCell() && isHTMLTableRowElement(*node));

  if (!parentIsTableElementPart)
    return true;

  EDisplay display = style.display();
  bool formIsTablePart =
      display == EDisplay::Table || display == EDisplay::InlineTable ||
      display == EDisplay::TableRowGroup ||
      display == EDisplay::TableHeaderGroup ||
      display == EDisplay::TableFooterGroup || display == EDisplay::TableRow ||
      display == EDisplay::TableColumnGroup ||
      display == EDisplay::TableColumn || display == EDisplay::TableCell ||
      display == EDisplay::TableCaption;

  return formIsTablePart;
}
