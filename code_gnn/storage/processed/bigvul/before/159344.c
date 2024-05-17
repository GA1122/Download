ChromeExtensionsAPIClient::CreateContentRulesRegistry(
    content::BrowserContext* browser_context,
    RulesCacheDelegate* cache_delegate) const {
  return scoped_refptr<ContentRulesRegistry>(
      new ChromeContentRulesRegistry(
          browser_context,
          cache_delegate,
          base::Bind(&CreateDefaultContentPredicateEvaluators,
                     base::Unretained(browser_context))));
}
