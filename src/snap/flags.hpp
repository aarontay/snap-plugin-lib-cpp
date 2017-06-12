#ifndef PLUGIN_FLAGS_HPP
#define PLUGIN_FLAGS_HPP

#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <iterator>

namespace po = boost::program_options;
using namespace std;

namespace Plugin {
    //helper function to simplify main part
    template<class T>
    ostream& operator<<(ostream& os, const vector<T>& v) {
        copy(v.begin(), v.end(), ostream_iterator<T>(os, " "));
        return os;
    };
    
    class Flags {
    public:
        int CmdLineOptions(int argc, char **argv) {
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
                if (ifs) {
                    store(parse_config_file(ifs, config_file_options), vm);
                    notify(vm);
                }

                if (vm.count("help")) {
                    cout << visible << endl;
                    exit(0);
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
    };
} // namespace Plugin

#endif // PLUGIN_FLAGS_HPP
