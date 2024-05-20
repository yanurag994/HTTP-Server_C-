#include <string>

struct Line {
  std::string method;
  std::string target;
  std::string version;
  Line() {}
  Line(const std::string& line) {
    size_t meth_end = line.find(" ");
    if (meth_end != std::string::npos) {
      method = line.substr(0, meth_end);
    }
    size_t tar_end = line.find(" ", meth_end + 1);
    if (tar_end != std::string::npos) {
      target = line.substr(meth_end + 1, tar_end - meth_end - 1);
    }
    version = line.substr(tar_end + 1);
  }
};

struct Request {
  Line line;
  std::string headers;
  std::string body;

  Request(const std::string& req) {
    size_t line_end = req.find("\r\n");
    if (line_end != std::string::npos) {
      line = Line(req.substr(0, line_end));
      size_t headers_end = req.find("\r\n\r\n", line_end + 2);
      if (headers_end != std::string::npos) {
        headers = req.substr(line_end + 2, headers_end - line_end - 2);
        body = req.substr(headers_end + 4);
      }
      else {
        headers = req.substr(line_end + 2);
        body = "";
      }
    }
    else {
      line = req;
      headers = "";
      body = "";
    }
  }
};