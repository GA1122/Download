   bool CanCapture(const Extension& extension, const GURL& url) {
//   bool CanCapture(const Extension& extension,
//                   const GURL& url,
//                   extensions::CaptureRequirement capture_requirement) {
      return extension.permissions_data()->CanCaptureVisiblePage(
        url, kTabId, nullptr  );
//         url, kTabId, nullptr  , capture_requirement);
    }