static void t1_subset_end(PDF pdf)
{
    if (t1_synthetic) {
         
        while (!strstr(t1_line_array, "definefont")) {
            t1_getline();
            t1_putline(pdf);
        }
        while (!t1_end_eexec()) {
             
            t1_getline();
        }
         
        t1_putline(pdf);
    } else {
        while (!t1_end_eexec()) {
             
            t1_getline();
            t1_putline(pdf);
        }
    }
    t1_stop_eexec(pdf);
    if (fixedcontent) {
         
        while (!t1_cleartomark()) {
            t1_getline();
            t1_putline(pdf);
        }
         
        if (!t1_synthetic) {
             
            t1_check_end(pdf);
        }
    }
    get_length3();
}
