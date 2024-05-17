mojo::Handle TestServiceProvider::ConnectToService(
    const std::string& service_name) {
  EXPECT_EQ(1u, service_factories_.count(service_name))
      << "Unregistered service " << service_name << " requested.";
  mojo::MessagePipe pipe;
  std::map<std::string,
           base::Callback<void(mojo::ScopedMessagePipeHandle)> >::iterator it =
      service_factories_.find(service_name);
  if (it != service_factories_.end())
    it->second.Run(pipe.handle0.Pass());
  return pipe.handle1.release();
}
