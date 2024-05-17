  CrashNotificationDelegate(Profile* profile, const Extension* extension)
      : profile_(profile),
        is_hosted_app_(extension->is_hosted_app()),
        extension_id_(extension->id()) {
  }
