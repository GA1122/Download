static int close_input(h2_stream *stream) 
{
    switch (stream->state) {
        case H2_STREAM_ST_CLOSED_INPUT:
        case H2_STREAM_ST_CLOSED:
            return 0;  
        case H2_STREAM_ST_CLOSED_OUTPUT:
             
            set_state(stream, H2_STREAM_ST_CLOSED);
            break;
        default:
             
            set_state(stream, H2_STREAM_ST_CLOSED_INPUT);
            break;
    }
    return 1;
}
