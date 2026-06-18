// loadme.cpp : Stage 1 - Login Screen with Discord Authentication
// Professional Windows C++ Console Loader Application

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <thread>
#include <chrono>
#include <conio.h>

// ==================== ANIMATION MANAGER ====================
class AnimationManager {
public:
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Force black background on startup
    void initializeConsole() {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        
        // Fill entire screen with black
        COORD coordScreen = { 0, 0 };
        DWORD cCharsWritten;
        DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
        
        FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten);
        FillConsoleOutputAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, dwConSize, coordScreen, &cCharsWritten);
        
        SetConsoleCursorPosition(hConsole, coordScreen);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    void setTextColor(int color) {
        SetConsoleTextAttribute(hConsole, color);
    }

    void clearScreen() {
        system("cls");
    }

    void sleep(int milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    void centerText(const std::string& text) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int padding = (columns - (int)text.length()) / 2;
        std::cout << std::string(padding, ' ') << text << "\n";
    }

    void displayBanner() {
        clearScreen();
        setTextColor(FOREGROUND_CYAN);
        centerText("");
        centerText("======================================");
        centerText("  LOADME - PROFESSIONAL LOADER v2.0");
        centerText("======================================");
        centerText("");
        setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    void displaySuccessAnimation() {
        clearScreen();
        setTextColor(FOREGROUND_GREEN);
        centerText("");
        centerText("      ✓✓✓ LOGIN ACCEPTED ✓✓✓");
        centerText("");
        setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        sleep(1500);
    }

    void displayErrorAnimation() {
        clearScreen();
        setTextColor(FOREGROUND_RED);
        
        for (int i = 0; i < 5; i++) {
            system("cls");
            centerText("");
            centerText("      ✗✗✗ LOGIN DENIED ✗✗✗");
            centerText("");
            sleep(200);
            
            system("cls");
            sleep(200);
        }
        
        system("cls");
        centerText("");
        centerText("      ✗✗✗ LOGIN DENIED ✗✗✗");
        centerText("");
        
        setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    void displayLoadingAnimation() {
        setTextColor(FOREGROUND_YELLOW);
        for (int i = 0; i < 6; i++) {
            std::cout << ".";
            std::cout.flush();
            sleep(300);
        }
        std::cout << "\n";
        setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
};

// ==================== DISCORD AUTHENTICATION ====================
class DiscordAuth {
private:
    // Hardcoded Discord database (in production, this would be a real Discord bot/webhook)
    struct DiscordUser {
        std::string username;
        std::string password;
        bool valid;
    };

    std::vector<DiscordUser> discordDatabase = {
        { "admin", "password123", true },
        { "user", "test123", true },
        { "developer", "devpass456", true }
    };

public:
    bool validateWithDiscord(const std::string& username, const std::string& password) {
        std::cout << "\nConnecting to Discord authentication server";
        AnimationManager animator;
        animator.displayLoadingAnimation();

        // Simulate network delay
        animator.sleep(1000);

        // Check against Discord database
        for (const auto& user : discordDatabase) {
            if (user.username == username && user.password == password && user.valid) {
                std::cout << "[+] Discord verification successful\n";
                return true;
            }
        }

        std::cout << "[!] Discord verification failed\n";
        return false;
    }

    std::string getAuthenticationMethod() {
        return "Discord OAuth 2.0";
    }
};

// ==================== LOGIN MANAGER ====================
class LoginManager {
private:
    AnimationManager animator;
    DiscordAuth discordAuth;
    std::string authenticatedUser;
    bool isAuthenticated = false;
    int loginAttempts = 0;
    const int MAX_ATTEMPTS = 3;

public:
    LoginManager() {
        animator.initializeConsole();
    }

    std::string promptUsername() {
        std::string username;
        animator.setTextColor(FOREGROUND_CYAN);
        std::cout << "\n[?] Username: ";
        animator.setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        std::getline(std::cin, username);
        return username;
    }

    std::string promptPassword() {
        std::string password;
        animator.setTextColor(FOREGROUND_CYAN);
        std::cout << "[?] Password: ";
        animator.setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        char ch;
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
                if (!password.empty()) {
                    password.pop_back();
                    std::cout << "\b \b";
                }
            }
            else {
                password += ch;
                std::cout << '*';
            }
        }
        std::cout << "\n";
        return password;
    }

    bool validateInput(const std::string& username, const std::string& password) {
        if (username.empty() || password.empty()) {
            animator.setTextColor(FOREGROUND_RED);
            std::cout << "\n[!] Username and Password cannot be empty.\n";
            animator.setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            return false;
        }
        return true;
    }

    bool authenticate(const std::string& username, const std::string& password) {
        return discordAuth.validateWithDiscord(username, password);
    }

    bool login() {
        animator.displayBanner();

        while (loginAttempts < MAX_ATTEMPTS) {
            std::string username = promptUsername();
            std::string password = promptPassword();

            if (!validateInput(username, password)) {
                loginAttempts++;
                animator.displayBanner();
                continue;
            }

            if (!authenticate(username, password)) {
                loginAttempts++;
                animator.displayErrorAnimation();
                animator.sleep(3000);
                
                if (loginAttempts < MAX_ATTEMPTS) {
                    animator.displayBanner();
                }
                continue;
            }

            // Success
            authenticatedUser = username;
            isAuthenticated = true;
            animator.displaySuccessAnimation();
            return true;
        }

        // Max attempts exceeded
        animator.displayErrorAnimation();
        animator.sleep(3000);
        return false;
    }

    std::string getAuthenticatedUser() const {
        return authenticatedUser;
    }

    bool getAuthenticationStatus() const {
        return isAuthenticated;
    }
};

// ==================== GLOBAL STATE ====================
struct AppState {
    int currentStage = 1;
    std::string authenticatedUser;
    std::string authToken;
    std::string selectedGame;
    int selectedGamePID = -1;
};

// ==================== MAIN ====================
int main() {
    SetConsoleTitleA("Loadme - Professional Loader v2.0");

    AppState appState;
    LoginManager loginManager;
    AnimationManager animator;

    // Stage 1: Login with Discord Authentication
    if (!loginManager.login()) {
        animator.setTextColor(FOREGROUND_RED);
        std::cout << "\n[!] Maximum login attempts exceeded. Application closing...\n";
        animator.setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        animator.sleep(2000);
        return 1;
    }

    appState.currentStage = 2;
    appState.authenticatedUser = loginManager.getAuthenticatedUser();

    animator.setTextColor(FOREGROUND_GREEN);
    std::cout << "\n[+] User '" << appState.authenticatedUser << "' authenticated successfully.\n";
    std::cout << "[+] Initializing Stage 2 - Loader Initialization...\n";
    animator.setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    animator.sleep(1000);
    std::cout << "\n(Press Enter to continue to Stage 2...)\n";
    std::cin.get();

    return 0;
}
