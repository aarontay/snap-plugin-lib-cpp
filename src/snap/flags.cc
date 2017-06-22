#include <map>
#include <iostream>
#include <boost/any.hpp>
#include "flags.h"
//#include <snap/flags.h>

// Set default option values
#define STAND_ALONE_PORT 8181
#define LOG_LEVEL 2
#define OPTIONS_FILE "options.cfg"

namespace po = boost::program_options;

// Group of options allowed only on command line
int Plugin::Flags::addDefaultCommandFlags() {
    try {
        _command.add_options()
            ("help,h", "Display available command-line options")
            ("version,v", "Print the version");
        return 0;
    } 
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}

int Plugin::Flags::addDefaultGlobalFlags() {
    try {
        // Declare group of options
        // allowed both on command line and in config file
        _global.add_options()
            ("config", po::value<std::string>()->composing(), "Config to use in JSON format")
            ("port", po::value<std::string>()->composing(), "Port GRPC will listen on")
            ("pprof", "Enable pprof")
            ("tls", "Enable TLS")
            ("cert-path", po::value<std::string>()->composing(), "Necessary to provide when TLS enabled")
            ("key-path", po::value<std::string>()->composing(), "Necessary to provide when TLS enabled")
            ("root-cert-paths", po::value<std::string>()->composing(), "Root paths separator")
            ("stand-alone", "Enable stand-alone plugin")
            ("stand-alone-port", po::value<int>(&_stand_alone_port)->default_value(STAND_ALONE_PORT),
                "Specify http port when stand-alone is set")
            ("log-level", po::value<int>(&_log_level)->default_value(LOG_LEVEL), 
                "0:Panic 1:Fatal 2:Error 3:Warn 4:Info 5:Debug");
        _config_file.add(_global);
        return 0;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}

int Plugin::Flags::addDefaultHiddenFlags() {
    try {
        _hidden.add_options()
                ("options-file", po::value<string>(&_options_file)->default_value(OPTIONS_FILE),
                    "name of file for options configuration");
        return 0;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}

int Plugin::Flags::SetDefaultFlags() {
    try {
        if (addDefaultCommandFlags() != 0) return 1;
        if (addDefaultGlobalFlags() != 0) return 1;
        if (addDefaultHiddenFlags() != 0) return 1;
        return 0;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}

int Plugin::Flags::addBoolFlag(const char *optionName, const char *description,
                Plugin::Flags::FlagLevel flagLevel) {
    try {
        switch (flagLevel) {
        case Command:
            _command.add_options()
                    (optionName, description);
            break;
        case Global:
            _global.add_options()
                    (optionName, description);
            break;
        case Hidden:
            _hidden.add_options()
                    (optionName, description);
            break;
        case Custom:
            _additional.add_options()
                    (optionName, description);
            break;
        default:
            std::cout << "Flag level not supported" << std::endl;
            return 1;
        }
        return 0;
    } 
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
int Plugin::Flags::addIntFlag(const char *optionName, const char *description,
                    FlagLevel flagLevel) {
    try {
        switch (flagLevel) {
        case Command:
            _command.add_options()
                    (optionName, po::value<int>()->composing(), description);
            break;
        case Global:
            _global.add_options()
                    (optionName, po::value<int>()->composing(), description);
            break;
        case Hidden:
            _hidden.add_options()
                    (optionName, po::value<int>()->composing(), description);
            break;
        case Custom:
            _additional.add_options()
                    (optionName, po::value<int>()->composing(), description);
            break;
        default:
            std::cout << "Flag level not supported" << std::endl;
            return 1;
        }
        return 0;
    } 
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
int Plugin::Flags::addStringFlag(const char *optionName, const char *description,
                    FlagLevel flagLevel) {
                            try {
        switch (flagLevel) {
        case Command:
            _command.add_options()
                    (optionName, po::value<std::string>()->composing(), description);
            break;
        case Global:
            _global.add_options()
                    (optionName, po::value<std::string>()->composing(), description);
            break;
        case Hidden:
            _hidden.add_options()
                    (optionName, po::value<std::string>()->composing(), description);
            break;
        case Custom:
            _additional.add_options()
                    (optionName, po::value<std::string>()->composing(), description);
            break;
        default:
            std::cout << "Flag level not supported" << std::endl;
            return 1;
        }
        return 0;
    } 
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}

// Declare group of options allowed only on command line
int Plugin::Flags::AddFlag(const char * optionName, const char * description, 
                            FlagType flagType, Plugin::Flags::FlagLevel flagLevel) {
    try {
        switch (flagType) {
        case Bool:
            return addBoolFlag(optionName, description, flagLevel);
        case Int:
            return addIntFlag(optionName, description, flagLevel);
        case String:
            return addStringFlag(optionName, description, flagLevel);
        default:
            std::cout << "Flag type not supported" << std::endl;
            return 1;
        }
        return 0;
    } 
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}

int Plugin::Flags::setVisibleFlags() {
    try {
        _visible.add(_command).add(_global).add(_additional);
        return 0;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}

int Plugin::Flags::setCommandLineFlags() {
    try {
        _command_line.add(_command).add(_global).add(_additional).add(_hidden);
        return 0;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}

int Plugin::Flags::setConfigFileFlags() {
    try {
        _config_file.add(_global).add(_additional);
        return 0;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}

int Plugin::Flags::SetCombinedFlags() {
    try {
        if (setVisibleFlags() != 0) return 1;
        if (setCommandLineFlags() != 0) return 1;
        if (setConfigFileFlags() != 0) return 1;
        return 0;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}

int Plugin::Flags::ParseCommandLineFlags(const int &argc, char **argv) {
    try {
        po::store(po::command_line_parser(argc, argv).options(_command_line).run(), _flags);
        po::notify(_flags);

        if (_flags.count("help")) {
            exit(helpFlagCalled());
        }
        return 0;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}

int Plugin::Flags::ParseConfigFileFlags() {
    try {
        ifstream ifs(_options_file.c_str());
        if (ifs) {
            po::store(parse_config_file(ifs, _config_file), _flags);
            po::notify(_flags);
        }
        return 0;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}

int Plugin::Flags::ParseConfigFileFlags(std::string filePathAndName) {
    try {
        ifstream ifs(filePathAndName.c_str());
        if (ifs) {
            po::store(parse_config_file(ifs, _config_file), _flags);
            po::notify(_flags);
        }
        return 0;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}

void Plugin::Flags::ShowVariablesMap() {
   for (const auto& it : _flags) {
        std::cout << it.first.c_str() << " ";
        auto& value = it.second.value();
        if (auto v = boost::any_cast<int>(&value))
            std::cout << *v << std::endl;
        else if (auto v = boost::any_cast<std::string>(&value))
            std::cout << *v << std::endl;
        else
            std::cout << "error" << std::endl;
    }
}

int Plugin::Flags::helpFlagCalled() {
    std::cout << _visible << std::endl;
    return 0;
}