  void LoadResource(int idr) {
    ui::ResourceBundle& bundle = ui::ResourceBundle::GetSharedInstance();
    scoped_refptr<base::RefCountedMemory> resource =
        bundle.LoadDataResourceBytes(idr);
    RunTest(GURL(std::string("data:text/html,") +
                 std::string(resource->front_as<char>(), resource->size())));
  }
