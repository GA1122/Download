  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) {
    TabContents* tab_to_delete = tab_to_delete_;
    tab_to_delete_ = NULL;
    delete tab_to_delete;
  }
