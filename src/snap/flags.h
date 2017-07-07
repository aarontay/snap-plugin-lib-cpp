#pragma once

#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <iterator>
#include <spdlog/spdlog.h>

#define COMMAND_DESC "COMMANDS"
#define GLOBAL_DESC "GLOBAL OPTIONS"
#define ADDITIONAL_DESC ""

namespace po = boost::program_options;
namespace spd = spdlog;

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
        std::shared_ptr<spd::logger> _logger;

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

        Flags() {
            _logger = spdlog::stdout_logger_mt("flags");
        }

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

        int parseCommandLineFlags(const int &argc, char **argv);
        int parseConfigFileFlags(std::string filePathAndName = "");
        int ParseFlags(const int &argc, char **argv, std::string filePathAndName = "");

        void ShowVariablesMap();

        po::variables_map GetFlagsVM() { return _flags; }

        bool GetFlagBoolValue(const char *flagKey);
        int GetFlagIntValue(const char *flagKey);
        std::string GetFlagStrValue(const char *flagKey);

        void SetFlagsLogLevel(const int &logLevel = 2);

        int helpFlagCalled() {
            _logger->info(_visible);
            return 0;
        }

        po::options_description GetCommandOptions() { return _command; }
        void PrintCommandOptions() { _logger->info(_command); }

        po::options_description GetGlobalOptions() { return _global; }
        void PrintGlobalOptions() {_logger->info( _global); }

        po::options_description GetHiddenOptions() { return _hidden; }
        void PrintHiddenOptions() {_logger->info( _hidden); }

        po::options_description GetAdditionalOptions() { return _additional; }
        void PrintAdditionalOptions() { _logger->info(_additional); }

        po::options_description GetVisibleOptions() { return _visible; }
        void PrintVisibleOptions() { _logger->info(_visible); }

        po::options_description GetCommandLineOptions() { return _command_line; }
        void PrintCommandLineOptions() { _logger->info(_command_line); }

        po::options_description GetConfigFileOptions() { return _config_file; }
        void PrintConfigFileOptions() { _logger->info(_config_file); }
    };
} // namespace Plugin
