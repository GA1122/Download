  static void CreateForTesting(content::WebContents* web_contents) {
    web_contents->SetUserData(
        UserDataKey(),
        base::MakeUnique<TestSaveCardBubbleControllerImpl>(web_contents));
  }
