  void CreatePublicKeyCredentialWithUnsupportedAlgorithmAndExpectNotSupported(
      content::WebContents* web_contents) {
    std::string result;
    std::string script =
        "navigator.credentials.create({ publicKey: {"
        "  challenge: new TextEncoder().encode('climb a mountain'),"
        "  rp: { id: '1098237235409872', name: 'Acme' },"
        "  user: { "
        "    id: new TextEncoder().encode('1098237235409872'),"
        "    name: 'avery.a.jones@example.com',"
        "    displayName: 'Avery A. Jones', "
        "    icon: 'https://pics.acme.com/00/p/aBjjjpqPb.png'},"
        "  pubKeyCredParams: [{ type: 'public-key', alg: '123'}],"
        "  timeout: 60000,"
        "  excludeList: [] }"
        "}).catch(c => window.domAutomationController.send(c.toString()));";
    ASSERT_TRUE(
        content::ExecuteScriptAndExtractString(web_contents, script, &result));
    ASSERT_EQ(
        "NotSupportedError: Parameters for this operation are not supported.",
        result);
  }
