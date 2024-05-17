bool RunAllPending(content::WebContents* web_contents) {
  if (!content::ExecuteScript(web_contents, "1 == 1;"))
    return false;
  base::RunLoop().RunUntilIdle();
  return true;
}
