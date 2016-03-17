#include "inc/main.helper.hpp"

#include "inc/city.hpp"
#include "inc/server.hpp"
#include "inc/http_request.hpp"
#include "jsoncons/json.hpp"

#include <atomic>
#include <functional>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

#define CONST_CITY_WIDTH  10
#define CONST_CITY_HEIGHT 10

atomic<bool> _stop(false);
city* _city = new city(CONST_CITY_HEIGHT, CONST_CITY_WIDTH);

void thread_simulation_command_mode() {
    while(!_stop) {
        char c = getchar();
        if(c != '!') continue;
        _city->flow_pause();
        clear_screen();
        cout<<"Ready to take command [enter '$' to quit command mode]:"<<endl;
        const vector<string> commands = {
            "time_step(s)",
            "cluster_delays(ms)"
        };
        const vector<std::function<void(string)>> ops = {
            [](string s) { _city->time_step(stof(s)); },
            [](string s) { _city->cluster_delay(stof(s)); },
        };
        assert(commands.size() == ops.size());
        cout<<"List of commands:"<<endl;
        for(auto s : commands) cout << "    " << s << ": ?" << endl;
        string s;
        while(s != "$") {
            cin >> s;
            boost::trim(s);
            if(s.length() <= 1) continue;
            vector<string> spl;
            boost::split(spl, s, boost::is_any_of("=: "));
            if(spl.size() <= 1) { cout<<"!invalid!"<<endl; continue; }
            FOR(i,0,commands.size(),++) {
                if(boost::to_lower_copy(spl[0]) == commands[i]) {
                    ops[i](spl[1]);
                    break;
                }
            }
        }
        clear_screen();
        cout<<"Exiting command mode."<<endl;
        _city->flow_start();
    }
}

void thread_simulation() { return;
    boost::thread_group tg;
    tg.create_thread(thread_simulation_command_mode);
    cout<<_city->add_cars(10)<<endl;
    cout<<_city->status()<<endl;
    _city->flow_start();
    while(!_stop);
    tg.join_all();
}

string thread_proxy_ui_process_command(const jsoncons::json& json) {
    typedef const jsoncons::json& json_t;
    typedef pair<string, vector<string>> command_set;
    // consts
    const auto invalid_input = []() { throw jsoncons::json_exception_0<runtime_error>("invalid input!"); return ""; };
    const auto fields_exist = [&json](string name) { return json.find(name) != json.members().end(); };
    // define valid feild's valid commands
    const vector<command_set> commands = {
        command_set("op", { "get_info", "init", "feedback", "action", "help"}),
    };
    // define command feild's valid commands' handlers
    const vector<vector<std::function<string(json_t)>>> funcs {
        // command `op`
        {
            // get_info
            [](json_t) {
                using jsoncons::json;
                json jj;
                jj["grid_size"] = { _city->height(), _city->weight() };
                jj["street_count"] = _city->get_size_streets();
                jj["car_count"] = _city->get_size_cars();
                jj["time_step"] = _city->time_step();
                jj["cluster_delay"] = _city->cluster_delay();
                jj["flow"] = to_string(_city->get_stat_flow());
                return jj.to_string();
            },
            // init
            [&fields_exist, &invalid_input](json_t j) {
                using jsoncons::json;
                if(!fields_exist("params"))     invalid_input();
                _city->flow_stop();
                this_thread::sleep_for(chrono::seconds(3));
                json params = j["params"];
                for(auto c : { "size", "cars_no", "time_step", "cluster_delay" })
                    if(params.find(c) == params.members().end())
                        invalid_input();
                if(params["size"].size() != 2)
                    invalid_input();
                delete _city;
                _city = new city(params["size"][0].as<size_t>(), params["size"][1].as<size_t>());
                _city->add_cars(params["cars_no"].as<size_t>());
                _city->time_step(params["time_step"].as<float>());
                _city->cluster_delay(params["cluster_delay"].as<float>());
                json jj;
                jj["op"] = j["op"];
                jj["result"] = j["params"];
                jj["status"] = "OK";
                return jj.to_string();
            },
            // feedback
            [](json_t) {
                using jsoncons::json;
                json jj;
                jj["clusters"] = json::array(_city->get_size_cluster_street());
                size_t index0 = 0;
                for(const vector<street_ptr>& c: _city->cluster_streets()) {
                    auto cc = json::array(c.size());
                    size_t index1 = 0;
                    for(const street_ptr& s: c) {
                        json ss;
                        ss["name"] = s->name();
                        ss["capacity"] = s->capacity();
                        ss["length"] = s->length();
                        ss["traffic_weight"] = s->traffic_weight();
                        ss["size"] = { s->size(HEAD), s->size(TAIL) };
                        cc[index1++] = ss;
                    }
                    jj["clusters"][index0++] = cc;
                }
                return jj.to_string();
            },
            // action
            [&fields_exist, &invalid_input](json_t j) {
                using jsoncons::json;
                if(!fields_exist("action"))     invalid_input();
                if(j["action"] == "STOP")       _city->flow_stop();
                else if(j["action"] == "START") _city->flow_start();
                else if(j["action"] == "PAUSE") _city->flow_pause();
                else if(j["action"] == "time_step" && fields_exist("value")) _city->time_step(j["value"].as<float>());
                else if(j["action"] == "cluster_delay" && fields_exist("value")) _city->cluster_delay(j["value"].as<float>());
                else invalid_input();
                json jj;
                jj["op"] = j["op"];
                jj["result"] = to_string(_city->get_stat_flow());
                jj["status"] = "OK";
                return jj.to_string();
            },
            // help
            [&commands](json_t) {
                using jsoncons::json;
                json j;
                j["commands"] = json::array(commands.size());
                size_t index0 = 0;
                for(const command_set& c: commands) {
                    json jj;
                    jj["name"] = c.first;
                    jj["values"] = json::array(c.second.size());
                    size_t index1 = 0;
                    for(const string& s: c.second) jj["values"][index1++] = s;
                    j["commands"][index0++] = jj;
                }
                return j.to_string();
            }
        }
    };
    assert(commands.size() == funcs.size());
    // validate the SHOULD EXIST fields
    for(auto& f : commands) if(!fields_exist(f.first)) invalid_input();
    // iterate over ops
    FOR(i,0,commands.size(),++) {
        FOR(j,0,commands[i].second.size(),++)
        if(json[commands[i].first] == commands[i].second[j]) {
            return funcs[i][j](json);
        }
    }
    return invalid_input();
}

string thread_proxy_ui_process_request(const string& request) {
    try{
        auto json = jsoncons::json::parse(request);
        return thread_proxy_ui_process_command(json);

    } catch(const jsoncons::json_exception& e) {
        jsoncons::json j;
        j["code"]   = "422";
        j["result"] = "failed";
        j["detail"] = "Unprocessable Entity";
        return j.to_string();
    }
}

void thread_proxy_ui() {
    size_t sleep_time = 1;
    while(!_stop) {
        if(!_city) { this_thread::sleep_for(chrono::microseconds(100)); continue; }
        try
        {
            server h(2004);
            boost::system::error_code ec;
            while (true) {
                auto handle = h.accept();
                sleep_time = 1; // reset sleep time
                string in = h.receive(handle, ec, "\n");
                if(ec) { h.close(handle); continue; }
                h.send(handle, thread_proxy_ui_process_request(in), ec);
                h.close(handle);
            }
        }
        catch (std::exception& e)
        {
            cerr << "Exception: " << e.what() << endl;
            cerr << "Sleeping for " << sleep_time << " seconds...." << endl;
            this_thread::sleep_for(chrono::seconds(sleep_time));
            sleep_time = min(sleep_time * 2, size_t(5 * 60)); // max 5min sleep allowed!
        }
    }
}
