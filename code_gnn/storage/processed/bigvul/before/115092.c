void ScheduleCloseBalloon(const std::string& extension_id) {
  if (!MessageLoop::current())   
    return;
  MessageLoop::current()->PostTask(
      FROM_HERE, base::Bind(&CloseBalloon, kNotificationPrefix + extension_id));
}
