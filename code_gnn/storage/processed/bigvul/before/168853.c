bool TabsUpdateFunction::UpdateURL(const std::string &url_string,
                                   int tab_id,
                                   bool* is_async) {
  GURL url =
      ExtensionTabUtil::ResolvePossiblyRelativeURL(url_string, extension());

  if (!url.is_valid()) {
    error_ = ErrorUtils::FormatErrorMessage(
        keys::kInvalidUrlError, url_string);
    return false;
  }

  if (ExtensionTabUtil::IsKillURL(url)) {
    error_ = keys::kNoCrashBrowserError;
    return false;
  }

  if (url.SchemeIs(url::kJavaScriptScheme)) {
    if (!extension()->permissions_data()->CanAccessPage(
            extension(),
            web_contents_->GetURL(),
            tab_id,
            &error_)) {
      return false;
    }

    TabHelper::FromWebContents(web_contents_)
        ->script_executor()
        ->ExecuteScript(
            HostID(HostID::EXTENSIONS, extension_id()),
            ScriptExecutor::JAVASCRIPT,
            net::UnescapeURLComponent(
                url.GetContent(),
                net::UnescapeRule::URL_SPECIAL_CHARS_EXCEPT_PATH_SEPARATORS |
                    net::UnescapeRule::PATH_SEPARATORS |
                    net::UnescapeRule::SPACES),
            ScriptExecutor::SINGLE_FRAME, ExtensionApiFrameIdMap::kTopFrameId,
            ScriptExecutor::DONT_MATCH_ABOUT_BLANK, UserScript::DOCUMENT_IDLE,
            ScriptExecutor::MAIN_WORLD, ScriptExecutor::DEFAULT_PROCESS, GURL(),
            GURL(), user_gesture(), base::nullopt, ScriptExecutor::NO_RESULT,
            base::Bind(&TabsUpdateFunction::OnExecuteCodeFinished, this));

    *is_async = true;
    return true;
  }

  bool use_renderer_initiated = false;
  if (extension() && extension()->id() == extension_misc::kPdfExtensionId)
    use_renderer_initiated = true;
  NavigationController::LoadURLParams load_params(url);
  load_params.is_renderer_initiated = use_renderer_initiated;
  web_contents_->GetController().LoadURLWithParams(load_params);

  if (!url.SchemeIs(url::kJavaScriptScheme)) {
    DCHECK_EQ(
        url, web_contents_->GetController().GetPendingEntry()->GetVirtualURL());
  }

  return true;
}
