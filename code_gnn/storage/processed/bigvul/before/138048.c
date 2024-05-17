static AXSparseAttributeSetterMap& getSparseAttributeSetterMap() {
  DEFINE_STATIC_LOCAL(AXSparseAttributeSetterMap, axSparseAttributeSetterMap,
                      ());
  if (axSparseAttributeSetterMap.isEmpty()) {
    axSparseAttributeSetterMap.set(
        aria_activedescendantAttr,
        new ObjectAttributeSetter(AXObjectAttribute::AriaActiveDescendant));
    axSparseAttributeSetterMap.set(
        aria_controlsAttr,
        new ObjectVectorAttributeSetter(AXObjectVectorAttribute::AriaControls));
    axSparseAttributeSetterMap.set(
        aria_flowtoAttr,
        new ObjectVectorAttributeSetter(AXObjectVectorAttribute::AriaFlowTo));
    axSparseAttributeSetterMap.set(
        aria_detailsAttr,
        new ObjectVectorAttributeSetter(AXObjectVectorAttribute::AriaDetails));
    axSparseAttributeSetterMap.set(
        aria_errormessageAttr,
        new ObjectAttributeSetter(AXObjectAttribute::AriaErrorMessage));
    axSparseAttributeSetterMap.set(
        aria_keyshortcutsAttr,
        new StringAttributeSetter(AXStringAttribute::AriaKeyShortcuts));
    axSparseAttributeSetterMap.set(
        aria_roledescriptionAttr,
        new StringAttributeSetter(AXStringAttribute::AriaRoleDescription));
  }
  return axSparseAttributeSetterMap;
}
