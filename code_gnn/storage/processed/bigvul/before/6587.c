bool Smb4KMountJob::createMountAction(Smb4KShare *share, Action *action)
{
  Q_ASSERT(share);
  Q_ASSERT(action);
  
  if (!share || !action)
  {
    return false;
  }
  else
  {
  }
  
  QMap<QString, QVariant> mount_args;

  if (!fillArgs(share, mount_args))
  {
    return false;
  }
  else
  {
  }
  
  action->setName("net.sourceforge.smb4k.mounthelper.mount");
  action->setHelperID("net.sourceforge.smb4k.mounthelper");
  
  QMapIterator<QString, QVariant> it(mount_args);
  
  while (it.hasNext())
  {
    it.next();
    action->addArgument(it.key(), it.value());
  }
  
  if (!share->isHomesShare())
  {
    action->addArgument("mh_url", static_cast<QUrl>(share->url()));
  }
  else
  {
    action->addArgument("mh_url", static_cast<QUrl>(share->homeURL()));
  }  

  action->addArgument("mh_workgroup", share->workgroupName());
  action->addArgument("mh_comment", share->comment());
  action->addArgument("mh_ip", share->hostIP());

  if (QProcessEnvironment::systemEnvironment().contains("KRB5CCNAME"))
  {
    action->addArgument("mh_krb5ticket", QProcessEnvironment::systemEnvironment().value("KRB5CCNAME", ""));
  }
  else
  {
    QString ticket = QString("/tmp/krb5cc_%1").arg(KUser(KUser::UseRealUserID).uid());
    
    if (QFile::exists(ticket))
    {
      action->addArgument("mh_krb5ticket", "FILE:"+ticket);
    }
    else
    {
    }
  }
  
  return true;
}
