void FileManagerBrowserTestBase::EnableVirtualKeyboard() {
  CHECK(IsTabletModeTest());

  ash::mojom::ShellTestApiPtr shell_test_api;
  content::ServiceManagerConnection::GetForProcess()
      ->GetConnector()
      ->BindInterface(ash::mojom::kServiceName, &shell_test_api);
  ash::mojom::ShellTestApiAsyncWaiter waiter(shell_test_api.get());
  waiter.EnableVirtualKeyboard();
}
