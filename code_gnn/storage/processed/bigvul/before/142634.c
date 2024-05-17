  WebstoreInlineInstallerForTest(WebContents* contents,
                                 content::RenderFrameHost* host,
                                 const std::string& extension_id,
                                 const GURL& requestor_url,
                                 const Callback& callback)
      : WebstoreInlineInstaller(
            contents,
            host,
            kTestExtensionId,
            requestor_url,
            base::Bind(&WebstoreInlineInstallerForTest::InstallCallback,
                       base::Unretained(this))),
        install_result_target_(nullptr),
        programmable_prompt_(nullptr) {}
