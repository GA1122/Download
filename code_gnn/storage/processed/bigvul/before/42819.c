static void hide_next_step_button()
{
     
     
    gtk_widget_hide(g_btn_next);
     
    gtk_box_set_child_packing(g_box_buttons, g_btn_close, false, false, 5, GTK_PACK_END);
}
