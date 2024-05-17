void Navigate(NavigateParams* params) {
  Browser* source_browser = params->browser;
  AdjustNavigateParamsForURL(params);

  if (params->disposition == NEW_POPUP &&
      (source_browser && source_browser->window())) {
    params->disposition =
        source_browser->window()->GetDispositionForPopupBounds(
            params->window_bounds);
  }

  params->browser = GetBrowserForDisposition(params);

  if (!params->browser)
    return;

  if (params->browser->tab_count() >= browser_defaults::kMaxTabCount &&
      (params->disposition == NEW_POPUP ||
       params->disposition == NEW_FOREGROUND_TAB ||
       params->disposition == NEW_BACKGROUND_TAB)) {
      return;
  }


  if (GetSourceProfile(params, source_browser) != params->browser->profile()) {
    params->source_contents = NULL;
    params->referrer = GURL();
  }

  ScopedBrowserDisplayer displayer(params);

  ScopedTargetContentsOwner target_contents_owner(params);

  NormalizeDisposition(params);

  if (params->window_action == browser::NavigateParams::NO_ACTION &&
      source_browser != params->browser &&
      params->browser->tabstrip_model()->empty()) {
    params->window_action = browser::NavigateParams::SHOW_WINDOW;
  }

  if (params->window_action == browser::NavigateParams::SHOW_WINDOW &&
      params->disposition == NEW_POPUP &&
      params->user_gesture == false) {
    params->window_action = browser::NavigateParams::SHOW_WINDOW_INACTIVE;
  }

  content::PageTransition base_transition =
      content::PageTransitionStripQualifier(params->transition);
  bool user_initiated =
      params->transition & content::PAGE_TRANSITION_FROM_ADDRESS_BAR ||
      base_transition == content::PAGE_TRANSITION_TYPED ||
      base_transition == content::PAGE_TRANSITION_AUTO_BOOKMARK ||
      base_transition == content::PAGE_TRANSITION_GENERATED ||
      base_transition == content::PAGE_TRANSITION_START_PAGE ||
      base_transition == content::PAGE_TRANSITION_RELOAD ||
      base_transition == content::PAGE_TRANSITION_KEYWORD;

  std::string extra_headers;

  int singleton_index = GetIndexOfSingletonTab(params);

  if (!params->target_contents && singleton_index < 0) {
    GURL url;
    if (params->url.is_empty()) {
      url = params->browser->GetHomePage();
      params->transition = content::PageTransitionFromInt(
          params->transition | content::PAGE_TRANSITION_HOME_PAGE);
    } else {
      url = params->url;
    }

    if (params->disposition != CURRENT_TAB) {
      TabContents* source_contents = params->source_contents ?
          params->source_contents->tab_contents() : NULL;
      params->target_contents =
          Browser::TabContentsFactory(
              params->browser->profile(),
              tab_util::GetSiteInstanceForNewTab(
                  source_contents, params->browser->profile(), url),
              MSG_ROUTING_NONE,
              source_contents,
              NULL);
      target_contents_owner.TakeOwnership();
      params->target_contents->extension_tab_helper()->
          SetExtensionAppById(params->extension_app_id);
      if ((params->tabstrip_add_types & TabStripModel::ADD_ACTIVE) == 0) {
        params->target_contents->tab_contents()->WasHidden();
      }
    } else {
      params->target_contents = params->source_contents;
      DCHECK(params->target_contents);
    }

    if (user_initiated) {
      static_cast<RenderViewHostDelegate*>(params->target_contents->
          tab_contents())->OnUserGesture();
    }

    InitializeExtraHeaders(params, params->target_contents->profile(),
                           &extra_headers);

    if (!HandleNonNavigationAboutURL(url)) {
      if (params->is_renderer_initiated) {
        params->target_contents->controller().LoadURLFromRenderer(
            url, params->referrer, params->transition, extra_headers);
      } else {
        params->target_contents->controller().LoadURL(
            url, params->referrer, params->transition, extra_headers);
      }
    }
  } else {
  }

  if (params->source_contents &&
      (params->disposition == NEW_FOREGROUND_TAB ||
       params->disposition == NEW_WINDOW) &&
      (params->tabstrip_add_types & TabStripModel::ADD_INHERIT_OPENER))
    params->source_contents->tab_contents()->Focus();

  if (params->source_contents == params->target_contents) {
    params->browser->UpdateUIForNavigationInTab(
        params->target_contents,
        params->transition,
        user_initiated);
  } else if (singleton_index == -1) {
    if (params->tabstrip_index != -1)
      params->tabstrip_add_types |= TabStripModel::ADD_FORCE_INDEX;

    params->browser->tabstrip_model()->AddTabContents(
        params->target_contents,
        params->tabstrip_index,
        params->transition,
        params->tabstrip_add_types);
    target_contents_owner.ReleaseOwnership();
  }

  if (singleton_index >= 0) {
    TabContents* target = params->browser->GetTabContentsAt(singleton_index);

    if (target->is_crashed()) {
      target->controller().Reload(true);
    } else if (params->path_behavior == NavigateParams::IGNORE_AND_NAVIGATE &&
        target->GetURL() != params->url) {
      InitializeExtraHeaders(params, NULL, &extra_headers);
      if (params->is_renderer_initiated) {
        target->controller().LoadURLFromRenderer(
            params->url, params->referrer, params->transition, extra_headers);
      } else {
        target->controller().LoadURL(
            params->url, params->referrer, params->transition, extra_headers);
      }
    }

    if (params->source_contents != params->target_contents)
      params->browser->ActivateTabAt(singleton_index, user_initiated);
  }

  if (params->disposition != CURRENT_TAB) {
    NotificationService::current()->Notify(
        content::NOTIFICATION_TAB_ADDED,
        Source<TabContentsDelegate>(params->browser),
        Details<TabContents>(params->target_contents->tab_contents()));
  }
}
