  ActiveTabPermissionGranter* active_tab_permission_granter() {
    return extensions::TabHelper::FromWebContents(web_contents())->
        active_tab_permission_granter();
  }
