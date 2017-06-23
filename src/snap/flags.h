#ifndef PLUGIN_FLAGS_H
#define PLUGIN_FLAGS_H

#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <iterator>

#define COMMAND_DESC "COMMANDS"
#define GLOBAL_DESC "GLOBAL OPTIONS"
#define ADDITIONAL_DESC ""

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
    private:
        po::variables_map _flags;
        
        // Initial option descriptions
        po::options_description _command{COMMAND_DESC}, _global{GLOBAL_DESC};
        po::options_description _hidden, _additional{ADDITIONAL_DESC};
        
        // Combined option descriptions
        po::options_description _visible, _command_line, _config_file;

        // Default variables
        int _stand_alone_port, _log_level;
        std::string _options_file;

    public:
        enum FlagType {
            Bool,
            Int,
            String
        };

        enum FlagLevel {
            Command,
            Global,
            Hidden,
            Custom
        };

        int addDefaultCommandFlags();
        int addDefaultGlobalFlags();
        int addDefaultHiddenFlags();
        int SetDefaultFlags();

        int addBoolFlag(const char *optionName, const char *description,
                        FlagLevel flagLevel);
        int addIntFlag(const char *optionName, const char *description,
                        FlagLevel flagLevel);
        int addStringFlag(const char *optionName, const char *description,
                        FlagLevel flagLevel);
        int AddFlag(const char *optionName, const char *description, 
                        FlagType flagType, FlagLevel flagLevel);

        int setVisibleFlags();
        int setCommandLineFlags();
        int setConfigFileFlags();
        int SetCombinedFlags();

        int SetFlags() { 
            if (SetDefaultFlags() != 0) return 1;
            if (SetCombinedFlags() != 0) return 1;
            return 0;
        }

        int ParseCommandLineFlags(const int &argc, char **argv);
        int ParseConfigFileFlags();
        int ParseConfigFileFlags(std::string filePathAndName);

        void ShowVariablesMap();
        int helpFlagCalled();

        po::variables_map GetFlagsVM() { return _flags; }

        po::options_description GetCommandOptions() { return _command; }
        void PrintCommandOptions() { std::cout << _command << std::endl; }

        po::options_description GetGlobalOptions() { return _global; }
        void PrintGlobalOptions() { std::cout << _global << std::endl; }

        po::options_description GetHiddenOptions() { return _hidden; }
        void PrintHiddenOptions() { std::cout << _hidden << std::endl; }

        po::options_description GetAdditionalOptions() { return _additional; }
        void PrintAdditionalOptions() { std::cout << _additional << std::endl; }

        po::options_description GetVisibleOptions() { return _visible; }
        void PrintVisibleOptions() { std::cout << _visible << std::endl; }

        po::options_description GetCommandLineOptions() { return _command_line; }
        void PrintCommandLineOptions() { std::cout << _command_line << std::endl; }

        po::options_description GetConfigFileOptions() { return _config_file; }
        void PrintConfigFileOptions() { std::cout << _config_file << std::endl; }

    };
} // namespace Plugin

#endif // PLUGIN_FLAGS_H
