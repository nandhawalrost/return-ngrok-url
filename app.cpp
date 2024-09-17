#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string exec(const char* cmd) {
    std::string result = "";
    char buffer[128];
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) return "popen failed!";
    try {
        while (fgets(buffer, sizeof buffer, pipe) != nullptr) {
            result += buffer;
        }
    } catch (...) {
        _pclose(pipe);
        throw;
    }
    _pclose(pipe);
    return result;
}

std::string getNgrokAddress() {
    std::string cmd = "curl http://127.0.0.1:4040/api/tunnels";
    std::string response = exec(cmd.c_str());

    // Find the public URL
    std::size_t pos = response.find("\"public_url\":\"");
    if (pos != std::string::npos) {
        std::size_t start = pos + 14;  // Move past the "public_url":" part
        std::size_t end = response.find("\"", start);
        return response.substr(start, end - start);
        // return response; //return all
    }
    return "Ngrok address not found!";
}

int main() {
    std::string ngrokAddress = getNgrokAddress();
    std::cout << "Ngrok active address: " << ngrokAddress << std::endl;
    return 0;
}
