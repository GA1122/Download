static void t1_subset_end(void)
{
    if (t1_synthetic) {          
        while (!strstr(t1_line_array, "definefont")) {
            t1_getline();
            t1_putline();
        }
        while (!t1_end_eexec())
            t1_getline();        
        t1_putline();            
    } else
        while (!t1_end_eexec()) {        
            t1_getline();
            t1_putline();
        }
    t1_stop_eexec();
    if (fixedcontent) {          
        while (!t1_cleartomark()) {
            t1_getline();
            t1_putline();
        }
        if (!t1_synthetic)       
            t1_check_end();      
    }
    get_length3();
}
