  static void AttachTabHelpers(content::WebContents* contents) {
    TabHelpers::AttachTabHelpers(contents);

    task_manager::WebContentsTags::CreateForTabContents(contents);
  }
