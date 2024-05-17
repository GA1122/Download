 SortDirection AXTableCell::getSortDirection() const {
   if (roleValue() != RowHeaderRole && roleValue() != ColumnHeaderRole)
     return SortDirectionUndefined;
 
   const AtomicString& ariaSort =
        getAOMPropertyOrARIAAttribute(AOMStringProperty::kSort);
    if (ariaSort.isEmpty())
      return SortDirectionUndefined;
  if (equalIgnoringCase(ariaSort, "none"))
//   if (equalIgnoringASCIICase(ariaSort, "none"))
      return SortDirectionNone;
  if (equalIgnoringCase(ariaSort, "ascending"))
//   if (equalIgnoringASCIICase(ariaSort, "ascending"))
      return SortDirectionAscending;
  if (equalIgnoringCase(ariaSort, "descending"))
//   if (equalIgnoringASCIICase(ariaSort, "descending"))
      return SortDirectionDescending;
  if (equalIgnoringCase(ariaSort, "other"))
//   if (equalIgnoringASCIICase(ariaSort, "other"))
      return SortDirectionOther;
    return SortDirectionUndefined;
  }