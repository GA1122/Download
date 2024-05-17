  TabContents* CreateTabContents() {
    return chrome::TabContentsFactory(profile(), NULL, MSG_ROUTING_NONE, NULL);
  }
