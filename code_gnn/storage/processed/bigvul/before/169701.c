V8ValueConverterImpl::V8ValueConverterImpl()
    : date_allowed_(false),
      reg_exp_allowed_(false),
      function_allowed_(false),
      strip_null_from_objects_(false),
      avoid_identity_hash_for_testing_(false),
      strategy_(NULL) {}
