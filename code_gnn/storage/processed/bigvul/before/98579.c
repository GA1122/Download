bool GetCurrentTabFunction::RunImpl() {
  DCHECK(dispatcher());

  TabContents* contents = dispatcher()->delegate()->associated_tab_contents();
  if (contents)
    result_.reset(ExtensionTabUtil::CreateTabValue(contents));

  return true;
}
