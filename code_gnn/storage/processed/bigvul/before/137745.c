void CreateTabSocketMojoServiceForContents(
    HeadlessWebContents* web_contents,
    mojo::ScopedMessagePipeHandle handle) {
  HeadlessWebContentsImpl::From(web_contents)
      ->CreateTabSocketMojoService(std::move(handle));
}
