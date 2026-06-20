#include <iostream>
#include <windows.h>
#include <string>

// Function to launch applications on startup
void launchApplication(const std::string& appPath, const std::string& appName) {
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(STARTUPINFO);

    if (CreateProcess(
        appPath.c_str(),
        NULL,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi)) {
        std::cout << appName << " launched successfully!" << std::endl;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        std::cerr << "Failed to launch " << appName << std::endl;
    }
}

int main() {
    std::cout << "Starting PC Launch Applications..." << std::endl;

    // Launch Discord
    // Common Discord installation path
    std::string discordPath = "C:\\Users\\" + std::string(getenv("USERNAME")) + "\\AppData\\Local\\Discord\\app-1.0.9153\\Discord.exe";
    launchApplication(discordPath, "Discord");

    // Alternative Discord path (if installed in Program Files)
    if (GetFileAttributes(discordPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        discordPath = "C:\\Program Files\\Discord\\Discord.exe";
        launchApplication(discordPath, "Discord");
    }

    // Launch Google Chrome
    std::string chromePath = "C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe";
    launchApplication(chromePath, "Google Chrome");

    // Alternative Chrome path (32-bit)
    if (GetFileAttributes(chromePath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        chromePath = "C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe";
        launchApplication(chromePath, "Google Chrome");
    }

    std::cout << "Applications launched!" << std::endl;
    return 0;
}
