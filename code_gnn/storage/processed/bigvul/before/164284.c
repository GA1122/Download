QuitRunLoopOnRequestResolved(base::RunLoop* run_loop) {
  auto callback = std::make_unique<permission_helper::RequestResolvedCallback>(
      base::BindRepeating([](base::RunLoop* run_loop, const PermissionIDSet&) {
        run_loop->Quit();
      }, run_loop));
  ActiveTabPermissionGranterDelegateChromeOS::
      SetRequestResolvedCallbackForTesting(callback.get());
  return callback;
}
