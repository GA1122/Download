static void webkit_web_frame_class_init(WebKitWebFrameClass* frameClass)
{
    webkit_init();

     
    webkit_web_frame_signals[CLEARED] = g_signal_new("cleared",
            G_TYPE_FROM_CLASS(frameClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__VOID,
            G_TYPE_NONE, 0);

    webkit_web_frame_signals[LOAD_COMMITTED] = g_signal_new("load-committed",
            G_TYPE_FROM_CLASS(frameClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__VOID,
            G_TYPE_NONE, 0);

     
    webkit_web_frame_signals[LOAD_DONE] = g_signal_new("load-done",
            G_TYPE_FROM_CLASS(frameClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__BOOLEAN,
            G_TYPE_NONE, 1,
            G_TYPE_BOOLEAN);

    webkit_web_frame_signals[TITLE_CHANGED] = g_signal_new("title-changed",
            G_TYPE_FROM_CLASS(frameClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            NULL,
            NULL,
            webkit_marshal_VOID__STRING,
            G_TYPE_NONE, 1,
            G_TYPE_STRING);

    webkit_web_frame_signals[HOVERING_OVER_LINK] = g_signal_new("hovering-over-link",
            G_TYPE_FROM_CLASS(frameClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            NULL,
            NULL,
            webkit_marshal_VOID__STRING_STRING,
            G_TYPE_NONE, 2,
            G_TYPE_STRING, G_TYPE_STRING);

     
    webkit_web_frame_signals[SCROLLBARS_POLICY_CHANGED] = g_signal_new("scrollbars-policy-changed",
            G_TYPE_FROM_CLASS(frameClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            0,
            g_signal_accumulator_true_handled,
            NULL,
            webkit_marshal_BOOLEAN__VOID,
            G_TYPE_BOOLEAN, 0);

     
    GObjectClass* objectClass = G_OBJECT_CLASS(frameClass);
    objectClass->finalize = webkit_web_frame_finalize;
    objectClass->get_property = webkit_web_frame_get_property;

     
    g_object_class_install_property(objectClass, PROP_NAME,
                                    g_param_spec_string("name",
                                                        _("Name"),
                                                        _("The name of the frame"),
                                                        NULL,
                                                        WEBKIT_PARAM_READABLE));

    g_object_class_install_property(objectClass, PROP_TITLE,
                                    g_param_spec_string("title",
                                                        _("Title"),
                                                        _("The document title of the frame"),
                                                        NULL,
                                                        WEBKIT_PARAM_READABLE));

    g_object_class_install_property(objectClass, PROP_URI,
                                    g_param_spec_string("uri",
                                                        _("URI"),
                                                        _("The current URI of the contents displayed by the frame"),
                                                        NULL,
                                                        WEBKIT_PARAM_READABLE));

     
    g_object_class_install_property(objectClass, PROP_LOAD_STATUS,
                                    g_param_spec_enum("load-status",
                                                      "Load Status",
                                                      "Determines the current status of the load",
                                                      WEBKIT_TYPE_LOAD_STATUS,
                                                      WEBKIT_LOAD_FINISHED,
                                                      WEBKIT_PARAM_READABLE));

     
    g_object_class_install_property(objectClass, PROP_HORIZONTAL_SCROLLBAR_POLICY,
                                    g_param_spec_enum("horizontal-scrollbar-policy",
                                                      _("Horizontal Scrollbar Policy"),
                                                      _("Determines the current policy for the horizontal scrollbar of the frame."),
                                                      GTK_TYPE_POLICY_TYPE,
                                                      GTK_POLICY_AUTOMATIC,
                                                      WEBKIT_PARAM_READABLE));

     
    g_object_class_install_property(objectClass, PROP_VERTICAL_SCROLLBAR_POLICY,
                                    g_param_spec_enum("vertical-scrollbar-policy",
                                                      _("Vertical Scrollbar Policy"),
                                                      _("Determines the current policy for the vertical scrollbar of the frame."),
                                                      GTK_TYPE_POLICY_TYPE,
                                                      GTK_POLICY_AUTOMATIC,
                                                      WEBKIT_PARAM_READABLE));

    g_type_class_add_private(frameClass, sizeof(WebKitWebFramePrivate));
}
