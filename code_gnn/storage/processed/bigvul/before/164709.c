  ScopedInterfaceParamsReplacer(
      WebContents* web_contents,
      mojom::DidCommitProvisionalLoadInterfaceParamsPtr params_override)
      : DidCommitNavigationInterceptor(web_contents),
        params_override_(std::move(params_override)) {}
