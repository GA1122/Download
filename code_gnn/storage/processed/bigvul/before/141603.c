void DumpAccessibilityTreeTest::AddDefaultFilters(
    std::vector<Filter>* filters) {


  AddFilter(filters, "ALERT*");
  AddFilter(filters, "ANIMATED*");
  AddFilter(filters, "BUSY");
  AddFilter(filters, "CHECKED");
  AddFilter(filters, "COLLAPSED");
  AddFilter(filters, "EXPANDED");
  AddFilter(filters, "FLOATING");
  AddFilter(filters, "FOCUSABLE");
  AddFilter(filters, "HASPOPUP");
  AddFilter(filters, "INVISIBLE");
  AddFilter(filters, "MARQUEED");
  AddFilter(filters, "MIXED");
  AddFilter(filters, "MOVEABLE");
  AddFilter(filters, "MULTISELECTABLE");
  AddFilter(filters, "PRESSED");
  AddFilter(filters, "PROTECTED");
  AddFilter(filters, "READONLY");
  AddFilter(filters, "SELECTED");
  AddFilter(filters, "SIZEABLE");
  AddFilter(filters, "TRAVERSED");
  AddFilter(filters, "UNAVAILABLE");
  AddFilter(filters, "IA2_STATE_ACTIVE");
  AddFilter(filters, "IA2_STATE_ARMED");
  AddFilter(filters, "IA2_STATE_CHECKABLE");
  AddFilter(filters, "IA2_STATE_DEFUNCT");
  AddFilter(filters, "IA2_STATE_HORIZONTAL");
  AddFilter(filters, "IA2_STATE_ICONIFIED");
  AddFilter(filters, "IA2_STATE_INVALID_ENTRY");
  AddFilter(filters, "IA2_STATE_MODAL");
  AddFilter(filters, "IA2_STATE_MULTI_LINE");
  AddFilter(filters, "IA2_STATE_PINNED");
  AddFilter(filters, "IA2_STATE_REQUIRED");
  AddFilter(filters, "IA2_STATE_STALE");
  AddFilter(filters, "IA2_STATE_TRANSIENT");
  AddFilter(filters, "FOCUSED", Filter::DENY);
  AddFilter(filters, "HOTTRACKED", Filter::DENY);
  AddFilter(filters, "OFFSCREEN", Filter::DENY);
  AddFilter(filters, "value='*'");
  AddFilter(filters, "value='http*'", Filter::DENY);
  AddFilter(filters, "layout-guess:*", Filter::ALLOW);


  AddFilter(filters, "check*");
  AddFilter(filters, "descript*");
  AddFilter(filters, "collapsed");
  AddFilter(filters, "haspopup");
  AddFilter(filters, "horizontal");
  AddFilter(filters, "invisible");
  AddFilter(filters, "multiline");
  AddFilter(filters, "multiselectable");
  AddFilter(filters, "protected");
  AddFilter(filters, "required");
  AddFilter(filters, "select*");
  AddFilter(filters, "visited");
  AddFilter(filters, "busy=true");
  AddFilter(filters, "valueForRange*");
  AddFilter(filters, "minValueForRange*");
  AddFilter(filters, "maxValueForRange*");
  AddFilter(filters, "hierarchicalLevel*");
  AddFilter(filters, "autoComplete*");
  AddFilter(filters, "restriction*");
  AddFilter(filters, "keyShortcuts*");
  AddFilter(filters, "activedescendantId*");
  AddFilter(filters, "controlsIds*");
  AddFilter(filters, "flowtoIds*");
  AddFilter(filters, "detailsIds*");
  AddFilter(filters, "invalidState=*");
  AddFilter(filters, "invalidState=false",
            Filter::DENY);   
  AddFilter(filters, "roleDescription=*");
  AddFilter(filters, "errormessageId=*");


  AddFilter(filters, "AXValueAutofill*");
  AddFilter(filters, "AXAutocomplete*");


  AddFilter(filters, "hint=*");
  AddFilter(filters, "interesting", Filter::DENY);
  AddFilter(filters, "has_character_locations", Filter::DENY);
  AddFilter(filters, "has_image", Filter::DENY);


  AddFilter(filters, "*=''", Filter::DENY);
  AddFilter(filters, "name=*", Filter::ALLOW_EMPTY);
}
