bool ShouldTreatNavigationAsReload(const NavigationEntry* entry) {
  if (!entry)
    return false;

  if ((ui::PageTransitionCoreTypeIs(entry->GetTransitionType(),
                                    ui::PAGE_TRANSITION_RELOAD) &&
       (entry->GetTransitionType() & ui::PAGE_TRANSITION_FROM_ADDRESS_BAR))) {
    return true;
  }

  if (ui::PageTransitionCoreTypeIs(entry->GetTransitionType(),
                                   ui::PAGE_TRANSITION_TYPED)) {
    return true;
  }

  if (ui::PageTransitionCoreTypeIs(entry->GetTransitionType(),
                                   ui::PAGE_TRANSITION_LINK)) {
    return true;
  }
   return false;
 }
