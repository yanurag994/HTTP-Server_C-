#include "backend.hpp"

#include "request.hpp"
#include "response.hpp"
#include "compression.hpp"

void Backend::handle_request(int client) {
  std::string request(1024, '\0');
  recv(client, &request[0], request.length(), 0);

  Request req(request);
  Response response;

  if (req.line.method == "GET")
  {
    if (req.line.target.size() == 1 && req.line.target == "/")
    {
      response.line.set_status_code(200);
    }
    else if (req.line.target.size() >= 6 && req.line.target.substr(0, 6) == "/echo/")
    {
      response.line.set_status_code(200);
      response.headers.add_update_header("Content-Type", "text/plain");
      auto it = req.headers.find("Accept-Encoding");
      if (it != req.headers.end() && std::regex_search(it->second, std::regex("gzip"))) {
        response.headers.add_update_header("Content-Encoding", "gzip");
        std::string uncompressed_data = req.line.target.substr(6);
        std::vector<std::byte> compressed_data = compress(reinterpret_cast<const std::byte*>(uncompressed_data.data()), uncompressed_data.size());
        response.headers.add_update_header("Content-Type", "application/octet-stream");
        response.headers.add_update_header("Content-Length", std::to_string(compressed_data.size()));
        const char* resp = response.c_str();
        send(client, resp, strlen(resp), 0);
        send(client, compressed_data.data(), compressed_data.size(), 0);
        close(client);
        return;

      }
      else
      {
        response.body = req.line.target.substr(6);
      }
      response.headers.add_update_header("Content-Length", std::to_string(response.body.size()));
    }
    else if (req.line.target == "/user-agent")
    {
      response.line.set_status_code(200);
      response.headers.add_update_header("Content-Type", "text/plain");
      response.body = req.headers.at("User-Agent");
      response.headers.add_update_header("Content-Length", std::to_string(response.body.size()));
    }
    else if (req.line.target.size() >= 6 && req.line.target.substr(0, 7) == "/files/")
    {
      std::ifstream file(this->directory + req.line.target.substr(7));
      if (file.is_open())
      {
        response.line.set_status_code(200);
        response.headers.add_update_header("Content-Type", "application/octet-stream");
        response.body.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        response.headers.add_update_header("Content-Length", std::to_string(response.body.size()));
      }
      else
      {
        response.line.set_status_code(404);
      }
    }
    else {
      response.line.set_status_code(404);
    }
  }
  else if (req.line.method == "POST")
  {
    if (req.line.target.size() >= 6 && req.line.target.substr(0, 7) == "/files/")
    {
      std::ofstream file(this->directory + req.line.target.substr(7));
      if (file.is_open())
      {
        response.line.set_status_code(201);
        file.write(req.body.c_str(), std::stoi(req.headers.at("Content-Length")));
      }
      else
      {
        response.line.set_status_code(404);
      }
    }
  }

  const char* resp = response.c_str();
  send(client, resp, strlen(resp), 0);
  close(client);
  return;

}