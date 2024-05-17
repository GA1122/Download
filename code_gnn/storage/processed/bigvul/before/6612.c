const QString Smb4KGlobal::winsServer()
{
  QMap<QString,QString> global_opts = p->globalSambaOptions(false);
  QString wins_server;
  
  if (global_opts.contains("wins server"))
  {
    wins_server = global_opts.value("wins server");
  }
  else
  {
    if (global_opts.contains("wins support") &&
         (QString::compare(global_opts.value("wins support"), "yes", Qt::CaseInsensitive) == 0 ||
          QString::compare(global_opts.value("wins support"), "true", Qt::CaseInsensitive) == 0))
    {
      wins_server = "127.0.0.1";
    }
    else
    {
    }
  }
  
  return wins_server;
}
