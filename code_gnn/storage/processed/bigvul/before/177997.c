  bool Smb4KMountJob::fillArgs(Smb4KShare *share, QMap<QString, QVariant>& map)
  {
  QString mount;
  QStringList paths;
  paths << "/bin";
  paths << "/sbin";
  paths << "/usr/bin";
  paths << "/usr/sbin";
  paths << "/usr/local/bin";
  paths << "/usr/local/sbin";
  for (int i = 0; i < paths.size(); ++i)
  {
    mount = KGlobal::dirs()->findExe("mount.cifs", paths.at(i));
    if (!mount.isEmpty())
    {
      map.insert("mh_command", mount);
      break;
    }
    else
    {
      continue;
    }
  }
//   const QString mount = findMountExecutable();
  
    if (mount.isEmpty())
    {
   paths << "/sbin";
   paths << "/usr/bin";
   paths << "/usr/sbin";
   paths << "/usr/local/bin";
   paths << "/usr/local/sbin";
    }
    
//   map.insert("mh_command", mount);
//   
    QMap<QString, QString> global_options = globalSambaOptions();
    Smb4KCustomOptions *options  = Smb4KCustomOptionsManager::self()->findOptions(share);
       break;
     }