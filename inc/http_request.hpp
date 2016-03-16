#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "stdafx.hpp"
#include <unordered_map>
#include <boost/algorithm/string.hpp>

class http_request {
    string _content_string;
    vector<string> _content;
    unordered_map<string, string> _headers;
public:
    http_request(const string& request) {
        bool content = false;
        vector<string> strs;
        boost::split(strs,request,boost::is_any_of("\n"));
        this->_headers["Type"] = boost::trim_copy(strs[0]);
        FOR(i,1,strs.size(),++) {
            boost::trim(strs[i]);
            if(!content) {
                if(!strs[i].length()) { content = true; continue; }
                vector<string> spl;
                boost::split(spl,strs[i],boost::is_any_of(":"));
                assert(spl.size() > 1);
                string s = "";
                FOR(i,1,spl.size(),++) s += ":"+spl[i];
                this->_headers[spl[0]] = s.substr(1);
            } else
                this->_content.push_back(strs[i]);
        }
        this->_content_string = boost::join(this->content(), "\n");
    }
    inline vector<string> content() const { return this->_content;}
    inline string content_string() const { return this->_content_string; }
    inline string operator [](const string& k) const { return this->_headers.at(k); }
    inline bool header_exists(const string& k) const { return this->_headers.count(k); }
};

#endif // HTTP_REQUEST_H
