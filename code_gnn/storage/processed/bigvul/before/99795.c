TestRunner::TestRunner() : is_init_(false) {
  Init(JOB_LOCKDOWN, USER_RESTRICTED_SAME_ACCESS, USER_LOCKDOWN);
}
