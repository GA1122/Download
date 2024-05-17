int TestValidWindow(HWND window) {
  if (::IsWindow(window))
    return SBOX_TEST_SUCCEEDED;

  return SBOX_TEST_DENIED;
 }
