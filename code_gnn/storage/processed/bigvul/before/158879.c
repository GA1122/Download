  DialogPlusConsoleObserverDelegate(
      content::WebContentsDelegate* original_delegate,
      WebContents* web_contents,
      const std::string& filter)
      : content::ConsoleObserverDelegate(web_contents, filter),
        web_contents_(web_contents),
        original_delegate_(original_delegate) {}
