nsPluginInstance::getCmdLine(int hostfd, int controlfd)
{
    std::vector<std::string> arg_vec;

    std::string cmd = getGnashExecutable();
    if (cmd.empty()) {
        gnash::log_error("Failed to locate the Gnash executable!");
        return arg_vec;
    }
    arg_vec.push_back(cmd);

    arg_vec.push_back("-u");
    arg_vec.push_back(_swf_url);
    
    std::string pageurl = getCurrentPageURL();
    if (pageurl.empty()) {
        gnash::log_error("Could not get current page URL!");
    } else {
        arg_vec.push_back("-U");
        arg_vec.push_back(pageurl);
    }

    setupCookies(pageurl);
    setupProxy(pageurl);

    std::stringstream pars;
    pars << "-x "  <<  _window           
         << " -j " << _width             
         << " -k " << _height;            
#if GNASH_PLUGIN_DEBUG > 1
    pars << " -vv ";
#endif
    if ((hostfd > 0) && (controlfd)) {
        pars << " -F " << hostfd             
             << ":"    << controlfd;         
    }
    std::string pars_str = pars.str();
    typedef boost::char_separator<char> char_sep;
    boost::tokenizer<char_sep> tok(pars_str, char_sep(" "));
    arg_vec.insert(arg_vec.end(), tok.begin(), tok.end());

    for (std::map<std::string,std::string>::const_iterator it = _params.begin(),
        itEnd = _params.end(); it != itEnd; ++it) {
        const std::string& nam = it->first; 
        const std::string& val = it->second;
        arg_vec.push_back("-P");
        arg_vec.push_back(nam + "=" + val);
    }
    arg_vec.push_back("-");

    create_standalone_launcher(pageurl, _swf_url, _params);

    return arg_vec;
}
