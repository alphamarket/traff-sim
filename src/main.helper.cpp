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

city _city(CONST_CITY_HEIGHT, CONST_CITY_WIDTH);
atomic<bool> stop(false);

void thread_simulation_command_mode() {
    while(!stop) {
        char c = getchar();
        if(c != '!') continue;
        _city.flow_pause();
        clear_screen();
        cout<<"Ready to take command [enter '$' to quit command mode]:"<<endl;
        const vector<string> commands = {
            "time_step(s)",
            "cluster_delays(ms)"
        };
        const vector<std::function<void(string)>> ops = {
            [](string s) { _city.time_step(stof(s)); },
            [](string s) { _city.cluster_delay(stof(s)); },
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
        _city.flow_start();
    }
}

void thread_simulation() { return;
    boost::thread_group tg;
    tg.create_thread(thread_simulation_command_mode);
    cout<<_city.add_cars(10)<<endl;
    cout<<_city.status()<<endl;
    _city.flow_start();
    while(!stop);
    tg.join_all();
}

string thread_proxy_ui_process_command(const jsoncons::json& json) {
    return "[\"COMMAND!\"]";
}

string thread_proxy_ui_process_request(const http_request& hr) {
    try{
        auto json = jsoncons::json::parse(hr.content_string());
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
    while(!stop) {
        try
        {
            const string http_respond_header = "HTTP/1.x 200 OK\nConnection: close\nContent-Type: application/json\nAccess-Control-Allow-Origin: *\n";
            const auto get_http_output = [http_respond_header](string data) { return http_respond_header + "Content-Length: " + std::to_string(data.length()) + "\n\n" + data; };

            server h(2004);
            boost::system::error_code ec;
            while (true) {
                auto handle = h.accept();
                string in = h.receive(handle, ec);
                if(ec) { continue; }
                h.send(handle, get_http_output(thread_proxy_ui_process_request(http_request(in))), ec);
                h.close(handle);
            }
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }
}
