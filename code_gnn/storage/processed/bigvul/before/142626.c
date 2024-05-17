 TabHelper::TabHelper(content::WebContents* web_contents)
     : content::WebContentsObserver(web_contents),
       profile_(Profile::FromBrowserContext(web_contents->GetBrowserContext())),
      extension_app_(NULL),
      pending_web_app_action_(NONE),
      last_committed_nav_entry_unique_id_(0),
      update_shortcut_on_load_complete_(false),
      script_executor_(
          new ScriptExecutor(web_contents, &script_execution_observers_)),
      location_bar_controller_(new LocationBarController(web_contents)),
      extension_action_runner_(new ExtensionActionRunner(web_contents)),
      webstore_inline_installer_factory_(new WebstoreInlineInstallerFactory()),
      registry_observer_(this),
      image_loader_ptr_factory_(this),
      weak_ptr_factory_(this) {
  SessionTabHelper::CreateForWebContents(web_contents);
  web_contents->ForEachFrame(
      base::Bind(&TabHelper::SetTabId, base::Unretained(this)));
  active_tab_permission_granter_.reset(new ActiveTabPermissionGranter(
      web_contents,
      SessionTabHelper::IdForTab(web_contents),
      profile_));

  AddScriptExecutionObserver(ActivityLog::GetInstance(profile_));

  InvokeForContentRulesRegistries([this](ContentRulesRegistry* registry) {
    registry->MonitorWebContentsForRuleEvaluation(this->web_contents());
  });

  ChromeExtensionWebContentsObserver::CreateForWebContents(web_contents);
  ExtensionWebContentsObserver::GetForWebContents(web_contents)->dispatcher()->
      set_delegate(this);

  registrar_.Add(this,
                 content::NOTIFICATION_LOAD_STOP,
                 content::Source<NavigationController>(
                     &web_contents->GetController()));
}
