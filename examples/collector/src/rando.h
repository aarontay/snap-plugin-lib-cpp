/*
http://www.apache.org/licenses/LICENSE-2.0.txt
Copyright 2016 Intel Corporation
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#pragma once

#include <vector>
#include <iostream>

#include <snap/config.h>
#include <snap/metric.h>
#include <snap/plugin.h>
#include <snap/flags.h>

class Rando final : public Plugin::CollectorInterface {
    private:
        Plugin::Flags _cli;
    
    public:
    Rando(const int &argc, char **argv) { 
    //    _cli.SetFlags();
    //    _cli.ParseFlags(argc, argv);
    }
    const Plugin::ConfigPolicy get_config_policy();
    std::vector<Plugin::Metric> get_metric_types(Plugin::Config cfg);
    void collect_metrics(std::vector<Plugin::Metric> &metrics);
};
