static void rpng_x_cleanup(void)
{
 if (image_data) {
        free(image_data);
        image_data = NULL;
 }

 if (ximage) {
 if (ximage->data) {
            free(ximage->data);  
            ximage->data = (char *)NULL;  
 }
 XDestroyImage(ximage);
        ximage = NULL;
 }

 if (have_gc)
 XFreeGC(display, gc);

 if (have_window)
 XDestroyWindow(display, window);

 if (have_colormap)
 XFreeColormap(display, colormap);

 if (have_nondefault_visual)
 XFree(visual_list);
}
