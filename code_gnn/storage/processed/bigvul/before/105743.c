  static void SetImeConfigCallback(GObject* source_object,
                                   GAsyncResult* res,
                                   gpointer user_data) {
    IBusConfig* config = IBUS_CONFIG(user_data);
    g_return_if_fail(config);

    GError* error = NULL;
    const gboolean result =
        ibus_config_set_value_async_finish(config, res, &error);

    if (!result) {
      std::string message = "(unknown error)";
      if (error && error->message) {
        message = error->message;
      }
      LOG(ERROR) << "ibus_config_set_value_async failed: " << message;
    }

    if (error) {
      g_error_free(error);
    }
     g_object_unref(config);
   }
