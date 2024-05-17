static void webkit_web_view_class_init(WebKitWebViewClass* webViewClass)
{
    GtkBindingSet* binding_set;

    webkitInit();

     

     
    webkit_web_view_signals[CREATE_WEB_VIEW] = g_signal_new("create-web-view",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            G_STRUCT_OFFSET (WebKitWebViewClass, create_web_view),
            webkit_signal_accumulator_object_handled,
            NULL,
            webkit_marshal_OBJECT__OBJECT,
            WEBKIT_TYPE_WEB_VIEW , 1,
            WEBKIT_TYPE_WEB_FRAME);

     
    webkit_web_view_signals[WEB_VIEW_READY] = g_signal_new("web-view-ready",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            G_STRUCT_OFFSET (WebKitWebViewClass, web_view_ready),
            g_signal_accumulator_true_handled,
            NULL,
            webkit_marshal_BOOLEAN__VOID,
            G_TYPE_BOOLEAN, 0);

     
    webkit_web_view_signals[CLOSE_WEB_VIEW] = g_signal_new("close-web-view",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            G_STRUCT_OFFSET (WebKitWebViewClass, close_web_view),
            g_signal_accumulator_true_handled,
            NULL,
            webkit_marshal_BOOLEAN__VOID,
            G_TYPE_BOOLEAN, 0);

     
    webkit_web_view_signals[NAVIGATION_REQUESTED] = g_signal_new("navigation-requested",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            G_STRUCT_OFFSET (WebKitWebViewClass, navigation_requested),
            webkit_navigation_request_handled,
            NULL,
            webkit_marshal_ENUM__OBJECT_OBJECT,
            WEBKIT_TYPE_NAVIGATION_RESPONSE, 2,
            WEBKIT_TYPE_WEB_FRAME,
            WEBKIT_TYPE_NETWORK_REQUEST);

     
    webkit_web_view_signals[NEW_WINDOW_POLICY_DECISION_REQUESTED] =
        g_signal_new("new-window-policy-decision-requested",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            g_signal_accumulator_true_handled,
            NULL,
            webkit_marshal_BOOLEAN__OBJECT_OBJECT_OBJECT_OBJECT,
            G_TYPE_BOOLEAN, 4,
            WEBKIT_TYPE_WEB_FRAME,
            WEBKIT_TYPE_NETWORK_REQUEST,
            WEBKIT_TYPE_WEB_NAVIGATION_ACTION,
            WEBKIT_TYPE_WEB_POLICY_DECISION);

     
    webkit_web_view_signals[NAVIGATION_POLICY_DECISION_REQUESTED] = g_signal_new("navigation-policy-decision-requested",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            g_signal_accumulator_true_handled,
            NULL,
            webkit_marshal_BOOLEAN__OBJECT_OBJECT_OBJECT_OBJECT,
            G_TYPE_BOOLEAN, 4,
            WEBKIT_TYPE_WEB_FRAME,
            WEBKIT_TYPE_NETWORK_REQUEST,
            WEBKIT_TYPE_WEB_NAVIGATION_ACTION,
            WEBKIT_TYPE_WEB_POLICY_DECISION);

     
    webkit_web_view_signals[MIME_TYPE_POLICY_DECISION_REQUESTED] = g_signal_new("mime-type-policy-decision-requested",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            g_signal_accumulator_true_handled,
            NULL,
            webkit_marshal_BOOLEAN__OBJECT_OBJECT_STRING_OBJECT,
            G_TYPE_BOOLEAN, 4,
            WEBKIT_TYPE_WEB_FRAME,
            WEBKIT_TYPE_NETWORK_REQUEST,
            G_TYPE_STRING,
            WEBKIT_TYPE_WEB_POLICY_DECISION);

     
    webkit_web_view_signals[WINDOW_OBJECT_CLEARED] = g_signal_new("window-object-cleared",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            G_STRUCT_OFFSET (WebKitWebViewClass, window_object_cleared),
            NULL,
            NULL,
            webkit_marshal_VOID__OBJECT_POINTER_POINTER,
            G_TYPE_NONE, 3,
            WEBKIT_TYPE_WEB_FRAME,
            G_TYPE_POINTER,
            G_TYPE_POINTER);

     
    webkit_web_view_signals[DOWNLOAD_REQUESTED] = g_signal_new("download-requested",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            g_signal_accumulator_true_handled,
            NULL,
            webkit_marshal_BOOLEAN__OBJECT,
            G_TYPE_BOOLEAN, 1,
            G_TYPE_OBJECT);

     
    webkit_web_view_signals[LOAD_STARTED] = g_signal_new("load-started",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__OBJECT,
            G_TYPE_NONE, 1,
            WEBKIT_TYPE_WEB_FRAME);

     
    webkit_web_view_signals[LOAD_COMMITTED] = g_signal_new("load-committed",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__OBJECT,
            G_TYPE_NONE, 1,
            WEBKIT_TYPE_WEB_FRAME);


     
    webkit_web_view_signals[LOAD_PROGRESS_CHANGED] = g_signal_new("load-progress-changed",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__INT,
            G_TYPE_NONE, 1,
            G_TYPE_INT);

     
    webkit_web_view_signals[LOAD_ERROR] = g_signal_new("load-error",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST),
            0,
            g_signal_accumulator_true_handled,
            NULL,
            webkit_marshal_BOOLEAN__OBJECT_STRING_POINTER,
            G_TYPE_BOOLEAN, 3,
            WEBKIT_TYPE_WEB_FRAME,
            G_TYPE_STRING,
            G_TYPE_POINTER);

     
    webkit_web_view_signals[LOAD_FINISHED] = g_signal_new("load-finished",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__OBJECT,
            G_TYPE_NONE, 1,
            WEBKIT_TYPE_WEB_FRAME);

     
    webkit_web_view_signals[ONLOAD_EVENT] = g_signal_new("onload-event",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__OBJECT,
            G_TYPE_NONE, 1,
            WEBKIT_TYPE_WEB_FRAME);

     
    webkit_web_view_signals[TITLE_CHANGED] = g_signal_new("title-changed",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            NULL,
            NULL,
            webkit_marshal_VOID__OBJECT_STRING,
            G_TYPE_NONE, 2,
            WEBKIT_TYPE_WEB_FRAME,
            G_TYPE_STRING);

     
    webkit_web_view_signals[HOVERING_OVER_LINK] = g_signal_new("hovering-over-link",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            NULL,
            NULL,
            webkit_marshal_VOID__STRING_STRING,
            G_TYPE_NONE, 2,
            G_TYPE_STRING,
            G_TYPE_STRING);

     
    webkit_web_view_signals[POPULATE_POPUP] = g_signal_new("populate-popup",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__OBJECT,
            G_TYPE_NONE, 1,
            GTK_TYPE_MENU);

     
    webkit_web_view_signals[PRINT_REQUESTED] = g_signal_new("print-requested",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            g_signal_accumulator_true_handled,
            NULL,
            webkit_marshal_BOOLEAN__OBJECT,
            G_TYPE_BOOLEAN, 1,
            WEBKIT_TYPE_WEB_FRAME);

    webkit_web_view_signals[STATUS_BAR_TEXT_CHANGED] = g_signal_new("status-bar-text-changed",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__STRING,
            G_TYPE_NONE, 1,
            G_TYPE_STRING);

     
    webkit_web_view_signals[ICON_LOADED] = g_signal_new("icon-loaded",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__STRING,
            G_TYPE_NONE, 1,
            G_TYPE_STRING);

     
    webkit_web_view_signals[CONSOLE_MESSAGE] = g_signal_new("console-message",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            G_STRUCT_OFFSET(WebKitWebViewClass, console_message),
            g_signal_accumulator_true_handled,
            NULL,
            webkit_marshal_BOOLEAN__STRING_INT_STRING,
            G_TYPE_BOOLEAN, 3,
            G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING);

     
    webkit_web_view_signals[SCRIPT_ALERT] = g_signal_new("script-alert",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            G_STRUCT_OFFSET(WebKitWebViewClass, script_alert),
            g_signal_accumulator_true_handled,
            NULL,
            webkit_marshal_BOOLEAN__OBJECT_STRING,
            G_TYPE_BOOLEAN, 2,
            WEBKIT_TYPE_WEB_FRAME, G_TYPE_STRING);

     
    webkit_web_view_signals[SCRIPT_CONFIRM] = g_signal_new("script-confirm",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            G_STRUCT_OFFSET(WebKitWebViewClass, script_confirm),
            g_signal_accumulator_true_handled,
            NULL,
            webkit_marshal_BOOLEAN__OBJECT_STRING_POINTER,
            G_TYPE_BOOLEAN, 3,
            WEBKIT_TYPE_WEB_FRAME, G_TYPE_STRING, G_TYPE_POINTER);

     
    webkit_web_view_signals[SCRIPT_PROMPT] = g_signal_new("script-prompt",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)G_SIGNAL_RUN_LAST,
            G_STRUCT_OFFSET(WebKitWebViewClass, script_prompt),
            g_signal_accumulator_true_handled,
            NULL,
            webkit_marshal_BOOLEAN__OBJECT_STRING_STRING_STRING,
            G_TYPE_BOOLEAN, 4,
            WEBKIT_TYPE_WEB_FRAME, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_POINTER);

     
    webkit_web_view_signals[SELECT_ALL] = g_signal_new("select-all",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            G_STRUCT_OFFSET(WebKitWebViewClass, select_all),
            NULL, NULL,
            g_cclosure_marshal_VOID__VOID,
            G_TYPE_NONE, 0);

     
    webkit_web_view_signals[CUT_CLIPBOARD] = g_signal_new("cut-clipboard",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            G_STRUCT_OFFSET(WebKitWebViewClass, cut_clipboard),
            NULL, NULL,
            g_cclosure_marshal_VOID__VOID,
            G_TYPE_NONE, 0);

     
    webkit_web_view_signals[COPY_CLIPBOARD] = g_signal_new("copy-clipboard",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            G_STRUCT_OFFSET(WebKitWebViewClass, copy_clipboard),
            NULL, NULL,
            g_cclosure_marshal_VOID__VOID,
            G_TYPE_NONE, 0);

     
    webkit_web_view_signals[PASTE_CLIPBOARD] = g_signal_new("paste-clipboard",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            G_STRUCT_OFFSET(WebKitWebViewClass, paste_clipboard),
            NULL, NULL,
            g_cclosure_marshal_VOID__VOID,
            G_TYPE_NONE, 0);

     
    webkit_web_view_signals[UNDO] = g_signal_new("undo",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            G_STRUCT_OFFSET(WebKitWebViewClass, undo),
            NULL, NULL,
            g_cclosure_marshal_VOID__VOID,
            G_TYPE_NONE, 0);

     
    webkit_web_view_signals[REDO] = g_signal_new("redo",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            G_STRUCT_OFFSET(WebKitWebViewClass, redo),
            NULL, NULL,
            g_cclosure_marshal_VOID__VOID,
            G_TYPE_NONE, 0);

     
    webkit_web_view_signals[MOVE_CURSOR] = g_signal_new("move-cursor",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            G_STRUCT_OFFSET(WebKitWebViewClass, move_cursor),
            NULL, NULL,
            webkit_marshal_BOOLEAN__ENUM_INT,
            G_TYPE_BOOLEAN, 2,
            GTK_TYPE_MOVEMENT_STEP,
            G_TYPE_INT);

     
    webkit_web_view_signals[PLUGIN_WIDGET] = g_signal_new("create-plugin-widget",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags) (G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            0,
            webkit_signal_accumulator_object_handled,
            NULL,
            webkit_marshal_OBJECT__STRING_STRING_POINTER,
            GTK_TYPE_WIDGET, 3,
            G_TYPE_STRING, G_TYPE_STRING, G_TYPE_HASH_TABLE);

     
    webkit_web_view_signals[DATABASE_QUOTA_EXCEEDED] = g_signal_new("database-quota-exceeded",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags) (G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            0,
            NULL, NULL,
            webkit_marshal_VOID__OBJECT_OBJECT,
            G_TYPE_NONE, 2,
            G_TYPE_OBJECT, G_TYPE_OBJECT);

     
    webkit_web_view_signals[RESOURCE_REQUEST_STARTING] = g_signal_new("resource-request-starting",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            0,
            NULL, NULL,
            webkit_marshal_VOID__OBJECT_OBJECT_OBJECT_OBJECT,
            G_TYPE_NONE, 4,
            WEBKIT_TYPE_WEB_FRAME,
            WEBKIT_TYPE_WEB_RESOURCE,
            WEBKIT_TYPE_NETWORK_REQUEST,
            WEBKIT_TYPE_NETWORK_RESPONSE);

     
    webkit_web_view_signals[GEOLOCATION_POLICY_DECISION_REQUESTED] = g_signal_new("geolocation-policy-decision-requested",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST),
            0,
            NULL, NULL,
            webkit_marshal_BOOLEAN__OBJECT_OBJECT,
            G_TYPE_BOOLEAN, 2,
            WEBKIT_TYPE_WEB_FRAME,
            WEBKIT_TYPE_GEOLOCATION_POLICY_DECISION);

     
    webkit_web_view_signals[GEOLOCATION_POLICY_DECISION_CANCELLED] = g_signal_new("geolocation-policy-decision-cancelled",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST),
            0,
            NULL, NULL,
            g_cclosure_marshal_VOID__OBJECT,
            G_TYPE_NONE, 1,
            WEBKIT_TYPE_WEB_FRAME);

     

     
    webkit_web_view_signals[DOCUMENT_LOAD_FINISHED] = g_signal_new("document-load-finished",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            0,
            NULL, NULL,
            g_cclosure_marshal_VOID__OBJECT,
            G_TYPE_NONE, 1,
            WEBKIT_TYPE_WEB_FRAME);

     
    webkit_web_view_signals[FRAME_CREATED] = g_signal_new("frame-created",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            0,
            NULL, NULL,
            g_cclosure_marshal_VOID__OBJECT,
            G_TYPE_NONE, 1,
            WEBKIT_TYPE_WEB_FRAME);

    webkit_web_view_signals[SHOULD_BEGIN_EDITING] = g_signal_new("should-begin-editing",
        G_TYPE_FROM_CLASS(webViewClass), static_cast<GSignalFlags>(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
        G_STRUCT_OFFSET(WebKitWebViewClass, should_allow_editing_action), g_signal_accumulator_first_wins, 0,
        webkit_marshal_BOOLEAN__OBJECT, G_TYPE_BOOLEAN, 1, WEBKIT_TYPE_DOM_RANGE);

    webkit_web_view_signals[SHOULD_END_EDITING] = g_signal_new("should-end-editing", G_TYPE_FROM_CLASS(webViewClass),
        static_cast<GSignalFlags>(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
        G_STRUCT_OFFSET(WebKitWebViewClass, should_allow_editing_action), g_signal_accumulator_first_wins, 0,
        webkit_marshal_BOOLEAN__OBJECT, G_TYPE_BOOLEAN, 1, WEBKIT_TYPE_DOM_RANGE);

    webkit_web_view_signals[SHOULD_INSERT_NODE] = g_signal_new("should-insert-node", G_TYPE_FROM_CLASS(webViewClass), 
        static_cast<GSignalFlags>(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
        G_STRUCT_OFFSET(WebKitWebViewClass, should_allow_editing_action), g_signal_accumulator_first_wins, 0,
        webkit_marshal_BOOLEAN__OBJECT_OBJECT_ENUM, G_TYPE_BOOLEAN,
        3, WEBKIT_TYPE_DOM_NODE, WEBKIT_TYPE_DOM_RANGE, WEBKIT_TYPE_INSERT_ACTION);

    webkit_web_view_signals[SHOULD_INSERT_TEXT] = g_signal_new("should-insert-text", G_TYPE_FROM_CLASS(webViewClass),
        static_cast<GSignalFlags>(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
        G_STRUCT_OFFSET(WebKitWebViewClass, should_allow_editing_action), g_signal_accumulator_first_wins, 0,
        webkit_marshal_BOOLEAN__STRING_OBJECT_ENUM, G_TYPE_BOOLEAN,
        3, G_TYPE_STRING, WEBKIT_TYPE_DOM_RANGE, WEBKIT_TYPE_INSERT_ACTION);

    webkit_web_view_signals[SHOULD_DELETE_RANGE] = g_signal_new("should-delete-range", G_TYPE_FROM_CLASS(webViewClass),
        static_cast<GSignalFlags>(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
        G_STRUCT_OFFSET(WebKitWebViewClass, should_allow_editing_action), g_signal_accumulator_first_wins, 0,
        webkit_marshal_BOOLEAN__OBJECT, G_TYPE_BOOLEAN, 1, WEBKIT_TYPE_DOM_RANGE);

    webkit_web_view_signals[SHOULD_SHOW_DELETE_INTERFACE_FOR_ELEMENT] = g_signal_new("should-show-delete-interface-for-element",
        G_TYPE_FROM_CLASS(webViewClass), static_cast<GSignalFlags>(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
        G_STRUCT_OFFSET(WebKitWebViewClass, should_allow_editing_action), g_signal_accumulator_first_wins, 0,
        webkit_marshal_BOOLEAN__OBJECT, G_TYPE_BOOLEAN, 1, WEBKIT_TYPE_DOM_HTML_ELEMENT);

    webkit_web_view_signals[SHOULD_CHANGE_SELECTED_RANGE] = g_signal_new("should-change-selected-range",
        G_TYPE_FROM_CLASS(webViewClass), static_cast<GSignalFlags>(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
        G_STRUCT_OFFSET(WebKitWebViewClass, should_allow_editing_action), g_signal_accumulator_first_wins, 0,
        webkit_marshal_BOOLEAN__OBJECT_OBJECT_ENUM_BOOLEAN, G_TYPE_BOOLEAN,
         4, WEBKIT_TYPE_DOM_RANGE, WEBKIT_TYPE_DOM_RANGE, WEBKIT_TYPE_SELECTION_AFFINITY, G_TYPE_BOOLEAN);

    webkit_web_view_signals[SHOULD_APPLY_STYLE] = g_signal_new("should-apply-style",
        G_TYPE_FROM_CLASS(webViewClass), static_cast<GSignalFlags>(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
        G_STRUCT_OFFSET(WebKitWebViewClass, should_allow_editing_action), g_signal_accumulator_first_wins, 0,
        webkit_marshal_BOOLEAN__OBJECT_OBJECT, G_TYPE_BOOLEAN,
         2, WEBKIT_TYPE_DOM_CSS_STYLE_DECLARATION, WEBKIT_TYPE_DOM_RANGE);

    webkit_web_view_signals[EDITING_BEGAN] = g_signal_new("editing-began",
        G_TYPE_FROM_CLASS(webViewClass), static_cast<GSignalFlags>(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION), 0, 0, 0,
        g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);

    webkit_web_view_signals[USER_CHANGED_CONTENTS] = g_signal_new("user-changed-contents",
        G_TYPE_FROM_CLASS(webViewClass), static_cast<GSignalFlags>(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION), 0, 0, 0,
        g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);

    webkit_web_view_signals[EDITING_ENDED] = g_signal_new("editing-ended",
        G_TYPE_FROM_CLASS(webViewClass), static_cast<GSignalFlags>(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION), 0, 0, 0,
        g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);

    webkit_web_view_signals[SELECTION_CHANGED] = g_signal_new("selection-changed",
        G_TYPE_FROM_CLASS(webViewClass), static_cast<GSignalFlags>(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION), 0, 0, 0,
        g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);

     
    webkit_web_view_signals[VIEWPORT_ATTRIBUTES_RECOMPUTE_REQUESTED] = g_signal_new("viewport-attributes-recompute-requested",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            0,
            0, 0,
            g_cclosure_marshal_VOID__OBJECT,
            G_TYPE_NONE, 1,
            WEBKIT_TYPE_VIEWPORT_ATTRIBUTES);

     
    webkit_web_view_signals[VIEWPORT_ATTRIBUTES_CHANGED] = g_signal_new("viewport-attributes-changed",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            0,
            0, 0,
            g_cclosure_marshal_VOID__OBJECT,
            G_TYPE_NONE, 1,
            WEBKIT_TYPE_VIEWPORT_ATTRIBUTES);

     
    webViewClass->create_web_view = webkit_web_view_real_create_web_view;
    webViewClass->web_view_ready = webkit_web_view_real_web_view_ready;
    webViewClass->close_web_view = webkit_web_view_real_close_web_view;
    webViewClass->navigation_requested = webkit_web_view_real_navigation_requested;
    webViewClass->window_object_cleared = webkit_web_view_real_window_object_cleared;
    webViewClass->choose_file = webkit_web_view_real_choose_file;
    webViewClass->script_alert = webkit_web_view_real_script_alert;
    webViewClass->script_confirm = webkit_web_view_real_script_confirm;
    webViewClass->script_prompt = webkit_web_view_real_script_prompt;
    webViewClass->console_message = webkit_web_view_real_console_message;
    webViewClass->select_all = webkit_web_view_real_select_all;
    webViewClass->cut_clipboard = webkit_web_view_real_cut_clipboard;
    webViewClass->copy_clipboard = webkit_web_view_real_copy_clipboard;
    webViewClass->paste_clipboard = webkit_web_view_real_paste_clipboard;
    webViewClass->undo = webkit_web_view_real_undo;
    webViewClass->redo = webkit_web_view_real_redo;
    webViewClass->move_cursor = webkit_web_view_real_move_cursor;
    webViewClass->should_allow_editing_action = webkit_web_view_real_should_allow_editing_action;

    GObjectClass* objectClass = G_OBJECT_CLASS(webViewClass);
    objectClass->dispose = webkit_web_view_dispose;
    objectClass->finalize = webkit_web_view_finalize;
    objectClass->get_property = webkit_web_view_get_property;
    objectClass->set_property = webkit_web_view_set_property;

    GtkWidgetClass* widgetClass = GTK_WIDGET_CLASS(webViewClass);
    widgetClass->realize = webkit_web_view_realize;
#ifdef GTK_API_VERSION_2
    widgetClass->expose_event = webkit_web_view_expose_event;
#else
    widgetClass->draw = webkit_web_view_draw;
#endif
    widgetClass->key_press_event = webkit_web_view_key_press_event;
    widgetClass->key_release_event = webkit_web_view_key_release_event;
    widgetClass->button_press_event = webkit_web_view_button_press_event;
    widgetClass->button_release_event = webkit_web_view_button_release_event;
    widgetClass->motion_notify_event = webkit_web_view_motion_event;
    widgetClass->scroll_event = webkit_web_view_scroll_event;
    widgetClass->size_allocate = webkit_web_view_size_allocate;
#ifdef GTK_API_VERSION_2
    widgetClass->size_request = webkit_web_view_size_request;
#else
    widgetClass->get_preferred_width = webkit_web_view_get_preferred_width;
    widgetClass->get_preferred_height = webkit_web_view_get_preferred_height;
#endif
    widgetClass->popup_menu = webkit_web_view_popup_menu_handler;
    widgetClass->grab_focus = webkit_web_view_grab_focus;
    widgetClass->focus_in_event = webkit_web_view_focus_in_event;
    widgetClass->focus_out_event = webkit_web_view_focus_out_event;
    widgetClass->get_accessible = webkit_web_view_get_accessible;
    widgetClass->screen_changed = webkit_web_view_screen_changed;
    widgetClass->drag_end = webkit_web_view_drag_end;
    widgetClass->drag_data_get = webkit_web_view_drag_data_get;
    widgetClass->drag_motion = webkit_web_view_drag_motion;
    widgetClass->drag_leave = webkit_web_view_drag_leave;
    widgetClass->drag_drop = webkit_web_view_drag_drop;
    widgetClass->drag_data_received = webkit_web_view_drag_data_received;
#if GTK_CHECK_VERSION(2, 12, 0)
    widgetClass->query_tooltip = webkit_web_view_query_tooltip;
    widgetClass->show_help = webkit_web_view_show_help;
#endif

    GtkContainerClass* containerClass = GTK_CONTAINER_CLASS(webViewClass);
    containerClass->add = webkit_web_view_container_add;
    containerClass->remove = webkit_web_view_container_remove;
    containerClass->forall = webkit_web_view_container_forall;

     
#ifdef GTK_API_VERSION_2
    webViewClass->set_scroll_adjustments = webkit_web_view_set_scroll_adjustments;
    GTK_WIDGET_CLASS(webViewClass)->set_scroll_adjustments_signal = g_signal_new("set-scroll-adjustments",
            G_TYPE_FROM_CLASS(webViewClass),
            (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
            G_STRUCT_OFFSET(WebKitWebViewClass, set_scroll_adjustments),
            NULL, NULL,
            webkit_marshal_VOID__OBJECT_OBJECT,
            G_TYPE_NONE, 2,
            GTK_TYPE_ADJUSTMENT, GTK_TYPE_ADJUSTMENT);
#else
    g_object_class_override_property(objectClass, PROP_HADJUSTMENT, "hadjustment");
    g_object_class_override_property(objectClass, PROP_VADJUSTMENT, "vadjustment");
    g_object_class_override_property(objectClass, PROP_HSCROLL_POLICY, "hscroll-policy");
    g_object_class_override_property(objectClass, PROP_VSCROLL_POLICY, "vscroll-policy");
#endif

     

    binding_set = gtk_binding_set_by_class(webViewClass);

    gtk_binding_entry_add_signal(binding_set, GDK_a, GDK_CONTROL_MASK,
                                 "select_all", 0);

     

    gtk_binding_entry_add_signal(binding_set, GDK_x, GDK_CONTROL_MASK,
                                 "cut_clipboard", 0);
    gtk_binding_entry_add_signal(binding_set, GDK_c, GDK_CONTROL_MASK,
                                 "copy_clipboard", 0);
    gtk_binding_entry_add_signal(binding_set, GDK_v, GDK_CONTROL_MASK,
                                 "paste_clipboard", 0);
    gtk_binding_entry_add_signal(binding_set, GDK_z, GDK_CONTROL_MASK,
                                 "undo", 0);
    gtk_binding_entry_add_signal(binding_set, GDK_z, static_cast<GdkModifierType>(GDK_CONTROL_MASK | GDK_SHIFT_MASK),
                                 "redo", 0);

    gtk_binding_entry_add_signal(binding_set, GDK_Delete, GDK_SHIFT_MASK,
                                 "cut_clipboard", 0);
    gtk_binding_entry_add_signal(binding_set, GDK_Insert, GDK_CONTROL_MASK,
                                 "copy_clipboard", 0);
    gtk_binding_entry_add_signal(binding_set, GDK_Insert, GDK_SHIFT_MASK,
                                 "paste_clipboard", 0);

     
    
    gtk_binding_entry_add_signal(binding_set, GDK_Down, static_cast<GdkModifierType>(0),
                                 "move-cursor", 2,
                                 G_TYPE_ENUM, GTK_MOVEMENT_DISPLAY_LINES,
                                 G_TYPE_INT, 1);
    gtk_binding_entry_add_signal(binding_set, GDK_Up, static_cast<GdkModifierType>(0),
                                 "move-cursor", 2,
                                 G_TYPE_ENUM, GTK_MOVEMENT_DISPLAY_LINES,
                                 G_TYPE_INT, -1);
    gtk_binding_entry_add_signal(binding_set, GDK_Right, static_cast<GdkModifierType>(0),
                                 "move-cursor", 2,
                                 G_TYPE_ENUM, GTK_MOVEMENT_VISUAL_POSITIONS,
                                 G_TYPE_INT, 1);
    gtk_binding_entry_add_signal(binding_set, GDK_Left, static_cast<GdkModifierType>(0),
                                 "move-cursor", 2,
                                 G_TYPE_ENUM, GTK_MOVEMENT_VISUAL_POSITIONS,
                                 G_TYPE_INT, -1);
    gtk_binding_entry_add_signal(binding_set, GDK_space, static_cast<GdkModifierType>(0),
                                 "move-cursor", 2,
                                 G_TYPE_ENUM, GTK_MOVEMENT_PAGES,
                                 G_TYPE_INT, 1);
    gtk_binding_entry_add_signal(binding_set, GDK_space, GDK_SHIFT_MASK,
                                 "move-cursor", 2,
                                 G_TYPE_ENUM, GTK_MOVEMENT_PAGES,
                                 G_TYPE_INT, -1);
    gtk_binding_entry_add_signal(binding_set, GDK_Page_Down, static_cast<GdkModifierType>(0),
                                 "move-cursor", 2,
                                 G_TYPE_ENUM, GTK_MOVEMENT_PAGES,
                                 G_TYPE_INT, 1);
    gtk_binding_entry_add_signal(binding_set, GDK_Page_Up, static_cast<GdkModifierType>(0),
                                 "move-cursor", 2,
                                 G_TYPE_ENUM, GTK_MOVEMENT_PAGES,
                                 G_TYPE_INT, -1);
    gtk_binding_entry_add_signal(binding_set, GDK_End, static_cast<GdkModifierType>(0),
                                 "move-cursor", 2,
                                 G_TYPE_ENUM, GTK_MOVEMENT_BUFFER_ENDS,
                                 G_TYPE_INT, 1);
    gtk_binding_entry_add_signal(binding_set, GDK_Home, static_cast<GdkModifierType>(0),
                                 "move-cursor", 2,
                                 G_TYPE_ENUM, GTK_MOVEMENT_BUFFER_ENDS,
                                 G_TYPE_INT, -1);

     

     
    g_object_class_install_property(objectClass, PROP_TITLE,
                                    g_param_spec_string("title",
                                                        _("Title"),
                                                        _("Returns the @web_view's document title"),
                                                        NULL,
                                                        WEBKIT_PARAM_READABLE));

     
    g_object_class_install_property(objectClass, PROP_URI,
                                    g_param_spec_string("uri",
                                                        _("URI"),
                                                        _("Returns the current URI of the contents displayed by the @web_view"),
                                                        NULL,
                                                        WEBKIT_PARAM_READABLE));

     
    g_object_class_install_property(objectClass, PROP_COPY_TARGET_LIST,
                                    g_param_spec_boxed("copy-target-list",
                                                       _("Copy target list"),
                                                       _("The list of targets this web view supports for clipboard copying"),
                                                       GTK_TYPE_TARGET_LIST,
                                                       WEBKIT_PARAM_READABLE));

     
    g_object_class_install_property(objectClass, PROP_PASTE_TARGET_LIST,
                                    g_param_spec_boxed("paste-target-list",
                                                       _("Paste target list"),
                                                       _("The list of targets this web view supports for clipboard pasting"),
                                                       GTK_TYPE_TARGET_LIST,
                                                       WEBKIT_PARAM_READABLE));

    g_object_class_install_property(objectClass, PROP_SETTINGS,
                                    g_param_spec_object("settings",
                                                        _("Settings"),
                                                        _("An associated WebKitWebSettings instance"),
                                                        WEBKIT_TYPE_WEB_SETTINGS,
                                                        WEBKIT_PARAM_READWRITE));

     
    g_object_class_install_property(objectClass, PROP_WEB_INSPECTOR,
                                    g_param_spec_object("web-inspector",
                                                        _("Web Inspector"),
                                                        _("The associated WebKitWebInspector instance"),
                                                        WEBKIT_TYPE_WEB_INSPECTOR,
                                                        WEBKIT_PARAM_READABLE));

     
    g_object_class_install_property(objectClass, PROP_VIEWPORT_ATTRIBUTES,
                                    g_param_spec_object("viewport-attributes",
                                                        _("Viewport Attributes"),
                                                        _("The associated WebKitViewportAttributes instance"),
                                                        WEBKIT_TYPE_VIEWPORT_ATTRIBUTES,
                                                        WEBKIT_PARAM_READABLE));

     
    g_object_class_install_property(objectClass, PROP_WINDOW_FEATURES,
                                    g_param_spec_object("window-features",
                                                        "Window Features",
                                                        "An associated WebKitWebWindowFeatures instance",
                                                        WEBKIT_TYPE_WEB_WINDOW_FEATURES,
                                                        WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(objectClass, PROP_EDITABLE,
                                    g_param_spec_boolean("editable",
                                                         _("Editable"),
                                                         _("Whether content can be modified by the user"),
                                                         FALSE,
                                                         WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(objectClass, PROP_TRANSPARENT,
                                    g_param_spec_boolean("transparent",
                                                         _("Transparent"),
                                                         _("Whether content has a transparent background"),
                                                         FALSE,
                                                         WEBKIT_PARAM_READWRITE));

     
    g_object_class_install_property(objectClass, PROP_ZOOM_LEVEL,
                                    g_param_spec_float("zoom-level",
                                                       _("Zoom level"),
                                                       _("The level of zoom of the content"),
                                                       G_MINFLOAT,
                                                       G_MAXFLOAT,
                                                       1.0f,
                                                       WEBKIT_PARAM_READWRITE));

     
    g_object_class_install_property(objectClass, PROP_FULL_CONTENT_ZOOM,
                                    g_param_spec_boolean("full-content-zoom",
                                                         _("Full content zoom"),
                                                         _("Whether the full content is scaled when zooming"),
                                                         FALSE,
                                                         WEBKIT_PARAM_READWRITE));

     
    g_object_class_install_property(objectClass, PROP_ENCODING,
                                    g_param_spec_string("encoding",
                                                        _("Encoding"),
                                                        _("The default encoding of the web view"),
                                                        NULL,
                                                        WEBKIT_PARAM_READABLE));

     
    g_object_class_install_property(objectClass, PROP_CUSTOM_ENCODING,
                                    g_param_spec_string("custom-encoding",
                                                        _("Custom Encoding"),
                                                        _("The custom encoding of the web view"),
                                                        NULL,
                                                        WEBKIT_PARAM_READWRITE));

     
    g_object_class_install_property(objectClass, PROP_LOAD_STATUS,
                                    g_param_spec_enum("load-status",
                                                      "Load Status",
                                                      "Determines the current status of the load",
                                                      WEBKIT_TYPE_LOAD_STATUS,
                                                      WEBKIT_LOAD_FINISHED,
                                                      WEBKIT_PARAM_READABLE));

     
    g_object_class_install_property(objectClass, PROP_PROGRESS,
                                    g_param_spec_double("progress",
                                                        "Progress",
                                                        "Determines the current progress of the load",
                                                        0.0, 1.0, 1.0,
                                                        WEBKIT_PARAM_READABLE));

     
    g_object_class_install_property(objectClass, PROP_ICON_URI,
                                    g_param_spec_string("icon-uri",
                                                        _("Icon URI"),
                                                        _("The URI for the favicon for the #WebKitWebView."),
                                                        NULL,
                                                        WEBKIT_PARAM_READABLE));
     
    g_object_class_install_property(objectClass, PROP_IM_CONTEXT,
                                    g_param_spec_object("im-context",
                                                        "IM Context",
                                                        "The GtkIMMultiContext for the #WebKitWebView.",
                                                        GTK_TYPE_IM_CONTEXT,
                                                        WEBKIT_PARAM_READABLE));

     
    g_object_class_install_property(objectClass, PROP_VIEW_MODE,
                                    g_param_spec_enum("view-mode",
                                                      "View Mode",
                                                      "The view-mode media feature for the #WebKitWebView.",
                                                      WEBKIT_TYPE_WEB_VIEW_VIEW_MODE,
                                                      WEBKIT_WEB_VIEW_VIEW_MODE_WINDOWED,
                                                      WEBKIT_PARAM_READWRITE));

    g_type_class_add_private(webViewClass, sizeof(WebKitWebViewPrivate));
}
