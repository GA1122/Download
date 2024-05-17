void RunCloseWithAppMenuCallback(Browser* browser) {
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(&CloseWindowCallback, browser));
  chrome::ShowAppMenu(browser);
}
