  GetUsageInfoTask(
      QuotaManager* manager,
      const GetUsageInfoCallback& callback)
      : QuotaTask(manager),
        callback_(callback),
        weak_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)) {
  }
