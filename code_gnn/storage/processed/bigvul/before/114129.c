on_ok_clicked(void *user_data, Evas_Object *obj, void *event_info)
{
    Eina_Bool *confirmed = (Eina_Bool *)user_data;
    *confirmed = EINA_TRUE;

    ecore_main_loop_quit();
}
