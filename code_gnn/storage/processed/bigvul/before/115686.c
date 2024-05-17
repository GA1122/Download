bool HostNPScriptObject::Localize(const NPVariant* args,
                                  uint32_t arg_count,
                                  NPVariant* result) {
  CHECK_EQ(base::PlatformThread::CurrentId(), np_thread_id_);
  if (arg_count != 1) {
    SetException("localize: bad number of arguments");
    return false;
  }

  if (NPVARIANT_IS_OBJECT(args[0])) {
    ScopedRefNPObject localize_func(NPVARIANT_TO_OBJECT(args[0]));
    LocalizeStrings(localize_func.get());
    return true;
  } else {
    SetException("localize: unexpected type for argument 1");
    return false;
  }
}
