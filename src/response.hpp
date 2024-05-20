#include <string>
#include <iostream>
#include <sstream>
#include <map>

struct Response_Line {
public:
  Response_Line() {
    version = "HTTP/1.1";
    set_status_code();
  }
  void set_status_code(unsigned short code = 500)
  {
    status_code = code;
    switch (status_code) {
    case 200:
      reason = "OK";
      break;
    case 404:
      reason = "Not Found";
      break;
    default:
      reason = "Something went wrong";
    }
  }
  friend std::ostream& operator<<(std::ostream& os, const Response_Line& response) {
    os << response.version << " " << response.status_code << " " << response.reason;
    return os;
  }
private:
  std::string version;
  unsigned short status_code;
  std::string reason;
};

struct Response_Headers {
public:
  Response_Headers() {
    headers = std::map<std::string, std::string>();
  }
  void add_update_header(const std::string& key, const std::string value) { headers[key] = value; }
  void remove_header(const std::string& key) { headers.erase(key); }
  friend std::ostream& operator<<(std::ostream& os, const Response_Headers& response) {
    for (auto& [key, value] : response.headers)
      os << key << ": " << value << "\r\n";
    return os;
  }
private:
  std::map<std::string, std::string> headers;
};

struct Response {
  Response_Line line;
  Response_Headers headers;
  std::string body;
  mutable std::string response_str; // Cache the string representation

  Response() {}
  friend std::ostream& operator<<(std::ostream& os, const Response& response) {
    os << response.line << "\r\n" << response.headers << "\r\n" << response.body;
    return os;
  }

  std::string to_string() const {
    std::ostringstream oss;
    oss << *this;
    response_str = oss.str(); // Cache the result in response_str
    return response_str;
  }

  const char* c_str() const {
    to_string(); // Update response_str
    return response_str.c_str();
  }
};