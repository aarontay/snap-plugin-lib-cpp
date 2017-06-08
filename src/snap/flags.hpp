#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <iterator>

namespace po = boost::program_options;
using namespace std;

namespace Plugin {
    class Flags {
    private:        
        // helper function to simplify main part
        template<class T>
        ostream& operator<<(ostream& os, const vector<T>& v) {
            copy(v.begin(), v.end(), ostream_iterator<T>(os, " "));
            return os;
        }

    public:
        int Options(int argc, const char *argv[]) {
            try {
                int sa_port, log_lev;
                string options_file;

                // Declare group of options
                // allowed only on command line
                po::options_description generic{"COMMANDS"};
                generic.add_options()
                    ("help,h", "Display available command-line options")
                    ("version,v", po::value<string>()->composing(), "Print the version");

                // Declare group of options
                // allowed both on command line and in config file
                po::options_description global_ops{"GLOBAL OPTIONS"};
                global_ops.add_options()
                    ("config", po::value<string>()->composing(), "Config to use in JSON format")
                    ("port", po::value<string>()->composing(), "Port GRPC will listen on")
                    ("pprof", "Enable pprof")
                    ("tls", "Enable TLS")
                    ("cert-path", po::value<string>()->composing(), "Necessary to provide when TLS enabled")
                    ("key-path", po::value<string>()->composing(), "Necessary to provide when TLS enabled")
                    ("root-cert-paths", po::value<string>()->composing(), "Root paths separator")
                    ("stand-alone", "Enable stand-alone plugin")
                    ("stand-alone-port", po::value<int>(&sa_port)->default_value(8181),
                        "Specify http port when stand-alone is set")
                    ("log-level", po::value<int>(&log_lev)->default_value(2), 
                        "0:Panic 1:Fatal 2:Error 3:Warn 4:Info 5:Debug");

                // Hidden options
                // allowed only on command line
                po::options_description hidden("Hidden options");
                hidden.add_options()
                    ("options-file", po::value<string>(&options_file)->default_value("options.cfg"),
                        "name of file for options configuration");
                
                po::options_description cmdline_options;
                cmdline_options.add(generic).add(global_ops).add(hidden);

                po::options_description config_file_options;
                config_file_options.add(global_ops);

                po::options_description visible;
                visible.add(generic).add(global_ops);

                // po::positional_options_description

                po::variables_map vm;
                // store(po::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
                store(po::command_line_parser(argc, argv).options(cmdline_options).run(), vm);
                notify(vm);

                ifstream ifs(options_file.c_str());
                if (!ifs) {
                    cout << "Can not open options configuration file: " << options_file << endl;
                    return 0;
                }
                else {
                    store(parse_config_file(ifs, config_file_options), vm);
                    notify(vm);
                }

                if (vm.count("help")) {
                    cout << visible << endl;
                    return 0;
                }

                if (vm.count("version")) {
                    cout << "Version X.XX.X" << endl;
                }

                if (vm.count("options-file")) {
                    cout << "Options configuration file is: " << vm["options-file"].as<string>() << endl;
                }

                if (vm.count("stand-alone")) {
                    cout << " Stand-alone enabled" << endl;
                }
                
            }
            catch (exception &e) {
                cout << e.what() << endl;
                return 1;
            }
            return 0;
        }
    } // namespace Flags
} // namespace Plugin


/*
std::string flConfig, flPort, flCertPath, flKeyPath, flRootCertPaths;
int flLogLevel = 2, flHTTPPort = 8181;
bool flPprof, flTLS, flStandAlone;

void read_command_line(int argc, char **argv) {
    opterr = 0;
    while(1) {
        static struct option long_options[] = 
        {
            {"config",           required_argument, 0, 'a'}, // config to use in JSON format
            {"port",             required_argument, 0, 'b'}, // port GRPC will listen on
            {"log-level",        required_argument, 0, 'c'}, // log level - 0:panic 1:fatal 2:error 3:warn 4:info 5:debug; default: 2
            {"pprof",            required_argument, 0, 'd'}, // enable pprof
            {"tls",              required_argument, 0, 'e'}, // enable TLS
            {"cert-path",        required_argument, 0, 'f'}, // necessary to provide when TLS enabled
            {"key-path",         required_argument, 0, 'g'}, // necessary to provide when TLS enabled
            {"root-key-paths",   required_argument, 0, 'h'}, // root paths separator
            {"stand-alone",      required_argument, 0, 'i'}, // enable stand alone plugin
            {"stand-alone-port", required_argument, 0, 'j'}, // specify http port when stand-alone is set
            {0, 0, 0, 0}
        };
        int option_index = 0;
        int i = getopt_long (argc, argv, "a:b:c:d:e:f:g:h:i:j")

        if (i == -1) break; // Detect end of options
        switch(i) {
            case 'a':
                
                break;
            case 'b':
                
                break;
            case 'c':
                
                break;      
            case 'd':
            
                break;
            case 'e':
                
                break;
            case 'f':
            
                break;
            case 'g':
            
                break;
            case 'h':
                
                break;
            case 'i':
                
                break;
            case 'j':

                break;
        }

    }
}
*/