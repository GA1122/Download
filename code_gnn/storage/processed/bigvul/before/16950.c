WebContext::BrowserContextDelegate::BrowserContextDelegate(
    const base::WeakPtr<WebContext> context)
    : context_getter_(new WebContextGetter(context)) {}
