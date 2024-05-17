  TabsCustomBindings::TabsCustomBindings(ScriptContext* context)
      : ObjectBackedNativeHandler(context) {
  RouteFunction("OpenChannelToTab",
      base::Bind(&TabsCustomBindings::OpenChannelToTab,
                 base::Unretained(this)));
//   RouteFunction("OpenChannelToTab", "tabs",
//                 base::Bind(&TabsCustomBindings::OpenChannelToTab,
//                            base::Unretained(this)));
  }