AccessibilityRole AXTableCell::determineAccessibilityRole() {
  if (!isTableCell())
    return AXLayoutObject::determineAccessibilityRole();

  return scanToDecideHeaderRole();
}
