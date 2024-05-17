test_scroll (void) {
    uzbl.gui.scbar_v = (GtkScrollbar*) gtk_vscrollbar_new (NULL);
    uzbl.gui.bar_v = gtk_range_get_adjustment((GtkRange*) uzbl.gui.scbar_v);

    gtk_adjustment_set_lower(uzbl.gui.bar_v, 0);
    gtk_adjustment_set_upper(uzbl.gui.bar_v, 100);
    gtk_adjustment_set_page_size(uzbl.gui.bar_v, 5);

     
    parse_cmd_line("scroll vertical end", NULL);
    g_assert_cmpfloat(gtk_adjustment_get_value(uzbl.gui.bar_v), ==, 95);

     
    parse_cmd_line("scroll vertical begin", NULL);
    g_assert_cmpfloat(gtk_adjustment_get_value(uzbl.gui.bar_v), ==, 0);

     
    parse_cmd_line("scroll vertical 15", NULL);
    g_assert_cmpfloat(gtk_adjustment_get_value(uzbl.gui.bar_v), ==, 15);

    parse_cmd_line("scroll vertical -10", NULL);
    g_assert_cmpfloat(gtk_adjustment_get_value(uzbl.gui.bar_v), ==, 5);

     
    parse_cmd_line("scroll vertical 100%", NULL);
    g_assert_cmpfloat(gtk_adjustment_get_value(uzbl.gui.bar_v), ==, 10);

    parse_cmd_line("scroll vertical 150%", NULL);
    g_assert_cmpfloat(gtk_adjustment_get_value(uzbl.gui.bar_v), ==, 17.5);

     
}