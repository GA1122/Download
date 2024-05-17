static int do_in_standby_l(struct stream_in *in)
{
 int status = 0;

 if (!in->standby) {

        in_close_pcm_devices(in);

        status = stop_input_stream(in);

 if (in->read_buf) {
            free(in->read_buf);
            in->read_buf = NULL;
 }

        in->standby = 1;
 }
 return 0;
}
