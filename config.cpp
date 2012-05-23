
/* Configuration stuff */

#include "config.hpp"

std::map<std::string, std::vector<std::string> > config::url_map;
std::map<std::string, int> config::max_downloads_map;

std::map<std::string, std::string> config::config_map;


int config::parse_config()
{
  /* Parses the configuration and puts the                                                                             
     data into url_map */

  

  cfg_opt_t urls[] =
    {
      CFG_STR_LIST("formats", "none", CFGF_NONE),
      CFG_INT("max_downloads", 1, CFGF_NONE),
      CFG_END()
    };
      
  cfg_opt_t opts[] =
    {
      CFG_SEC("url", urls, CFGF_TITLE | CFGF_MULTI),
      CFG_INT("debug", 0, CFGF_NONE),
      CFG_END()
    };


  cfg_t *cfg, *cfg_url;

  cfg = cfg_init(opts, CFGF_NONE);
  
  
  
  config_map["home"] = config::get_home();
  config_map["config_path"] = config_map["home"] + "/.clipodder/config";

  
  
  std::cout << "Looking in " << config_map["config_path"] << " for configuration" << std::endl;

  if(cfg_parse(cfg, config_map["config_path"].c_str()) == CFG_PARSE_ERROR)
    {
      std::cout << "Error parsing configuration" << std::endl;
      return 1;
    }

  

  
  dbg.set_state(cfg_getint(cfg, "debug"));
  
  
  


  /* Urls will be the title */
  int total_urls = cfg_size(cfg, "url");

  std::cout << total_urls << " urls found" << std::endl;
  
  

  /* Iterate through the urls and add the formats                                        
     and addresses to the url_map */
  for (int i = 0; i < total_urls; i++)
    {
      cfg_url = cfg_getnsec(cfg, "url", i);
      
      std::string addr = cfg_title(cfg_url);
      int num_formats = cfg_size(cfg_url, "formats");
      

      for (int b = 0; b < num_formats; b++)
	{
	  std::string format = cfg_getnstr(cfg_url, "formats", b);
	  
	  // map with vector
	  config::url_map[addr].push_back(format);
	}

      config::max_downloads_map[addr] = cfg_getint(cfg_url, "max_downloads");
      
    }
  
  return 0;
}


std::string config::get_home()
{
  //http://stackoverflow.com/questions/2910377/get-home-directory-in-linux-c

  std::string return_s;
  const char *home_dir;

  struct passwd *pw = getpwuid(getuid());

  home_dir = pw->pw_dir;
  
  return_s = home_dir;

  std::cout << "Home dir: " << return_s << std::endl;

  return return_s;
}
  
