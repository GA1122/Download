void ExtensionPrefs::SetLastPingDay(const std::string& extension_id,
                                    const Time& time) {
  DCHECK(Extension::IdIsValid(extension_id));
  ScopedExtensionPrefUpdate update(prefs_, extension_id);
  SaveTime(update.Get(), kLastPingDay, time);
}
