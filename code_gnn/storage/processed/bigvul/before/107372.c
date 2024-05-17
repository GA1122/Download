  FindInPageControllerTest() {
    EnableDOMAutomation();

#if defined(TOOLKIT_VIEWS)
    DropdownBarHost::disable_animations_during_testing_ = true;
#elif defined(TOOLKIT_GTK)
    SlideAnimatorGtk::SetAnimationsForTesting(false);
#elif defined(OS_MACOSX)
    FindBarBridge::disable_animations_during_testing_ = true;
#endif
  }
