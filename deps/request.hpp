#include <string>
#include <map>
#include <stdexcept>
#include <iostream>

struct Request_Line {
  std::string method;
  std::string target;
  std::string version;

  Request_Line() {}

  Request_Line(const std::string& line) {
    size_t meth_end = line.find(" ");
    if (meth_end == std::string::npos) {
      throw std::invalid_argument("Invalid request line: no method found");
    }
    method = line.substr(0, meth_end);

    size_t tar_end = line.find(" ", meth_end + 1);
    if (tar_end == std::string::npos) {
      throw std::invalid_argument("Invalid request line: no target found");
    }
    target = line.substr(meth_end + 1, tar_end - meth_end - 1);

    version = line.substr(tar_end + 1);
    if (version.empty()) {
      throw std::invalid_argument("Invalid request line: no version found");
    }
  }
};

struct Request {
  Request_Line line;
  std::map<std::string, std::string>  headers;
  std::string body;

  Request(const std::string& req) {
    size_t line_end = req.find("\r\n");
    if (line_end == std::string::npos) {
      throw std::invalid_argument("Invalid request: no request line found");
    }
    line = Request_Line(req.substr(0, line_end));

    size_t headers_end = req.rfind("\r\n");
    if (headers_end != std::string::npos) {
      parseHeaders(req.substr(line_end + 2, headers_end - line_end - 2));
      auto it = headers.find("Content-Length");
      if (it != headers.end() && std::stoi(it->second) > 0) {
        body = req.substr(headers_end + 2);
      }
      else {
        body = "";
      }
    }
    else {
      parseHeaders(req.substr(line_end + 2));
      body = "";
    }
  }
  void parseHeaders(const std::string& head) {
    size_t start = 0;
    while (start < head.size()) {
      size_t header_end = head.find("\r\n", start);
      if (header_end == std::string::npos) {
        break; 
      }

      std::string line = head.substr(start, header_end - start);
      size_t header_sep = line.find(": ");
      if (header_sep == std::string::npos) {
        throw std::invalid_argument("Invalid header line: " + line);
      }
      headers.emplace(line.substr(0, header_sep), line.substr(header_sep + 2));

      start = header_end + 2; 
    }
  }
};