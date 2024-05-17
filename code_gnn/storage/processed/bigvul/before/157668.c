void InputImeAPI::Observe(int type,
                          const content::NotificationSource& source,
                          const content::NotificationDetails& details) {
  DCHECK_EQ(chrome::NOTIFICATION_PROFILE_DESTROYED, type);
  extensions::InputImeEventRouterFactory::GetInstance()->RemoveProfile(
      content::Source<Profile>(source).ptr());
}
