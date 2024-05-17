int TestSwitchDesktop() {
  HDESK sbox_desk = ::GetThreadDesktop(::GetCurrentThreadId());
  if (NULL == sbox_desk) {
    return SBOX_TEST_FAILED;
  }
  if (::SwitchDesktop(sbox_desk)) {
    return SBOX_TEST_SUCCEEDED;
  }
   return SBOX_TEST_DENIED;
 }
