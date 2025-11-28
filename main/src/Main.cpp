/*This is a useless login system and there is no UI.*/
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <limits>
#include <algorithm>
#include <cctype>
#include <conio.h>

using namespace std;

// 颜色代码（ANSI）
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

class User {
public:
    string username;
    string password;
};

class Post {
public:
    string title;
    string content;
    string author;
    string timestamp;
    vector<string> comments;
};

class FileManager {
public:
    static vector<User> loadUsers() {
        vector<User> users;
        ifstream file("users.dat");
        string line;
        while (getline(file, line)) {
            size_t delim = line.find('|');
            if (delim != string::npos) {
                User user;
                user.username = line.substr(0, delim);
                user.password = line.substr(delim+1);
                users.push_back(user);
            }
        }
        file.close();
        return users;
    }

    static void saveUser(const User& user) {
        ofstream file("users.dat", ios::app);
        file << user.username << "|" << user.password << "\n";
        file.close();
    }

    static vector<Post> loadPosts() {
        vector<Post> posts;
        ifstream file("posts.dat");
        string line;
        while (getline(file, line)) {
            Post post;
            post.title = line;
            getline(file, post.author);
            getline(file, post.timestamp);
            getline(file, post.content);
            string comment;
            while (getline(file, comment) && !comment.empty()) {
                post.comments.push_back(comment);
            }
            posts.push_back(post);
        }
        file.close();
        return posts;
    }

    static void savePosts(const vector<Post>& posts) {
        ofstream file("posts.dat");
        for (const auto& post : posts) {
            file << post.title << "\n"
                 << post.author << "\n"
                 << post.timestamp << "\n"
                 << post.content << "\n";
            for (const auto& comment : post.comments) {
                file << comment << "\n";
            }
            file << "|\n"; // 帖子分隔符
        }
        file.close();
    }
};

class AuthManager {
public:
    User currentUser;

    bool registerUser() {
        User newUser;
        cout << CYAN << "\n创建新用户\n" << RESET;
        cout << "用户名: ";
        getline(cin, newUser.username);

        if (userExists(newUser.username)) {
            cout << RED << "用户名已存在！" << RESET << endl;
            return false;
        }

        cout << "密码: ";
        newUser.password = getMaskedInput();

        FileManager::saveUser(newUser);
        cout << GREEN << "注册成功！" << RESET << endl;
        return true;
    }

    bool login() {
        User user;
        cout << CYAN << "\n用户登录\n" << RESET;
        cout << "用户名: ";
        getline(cin, user.username);
        cout << "密码: ";
        user.password = getMaskedInput();

        vector<User> users = FileManager::loadUsers();
        auto it = find_if(users.begin(), users.end(), [&](const User& u) {
            return u.username == user.username && u.password == user.password;
        });

        if (it != users.end()) {
            currentUser = *it;
            cout << GREEN << "\n登录成功！" << RESET << endl;
            return true;
        }
        cout << RED << "无效的凭证！" << RESET << endl;
        return false;
    }

private:
    string getMaskedInput() {
        string password;
        char ch;
        while ((ch = _getch()) != '\r') { // Windows特定实现
            if (ch == '\b') { // 处理退格
                if (!password.empty()) {
                    cout << "\b \b";
                    password.pop_back();
                }
            } else {
                password.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;
        return password;
    }

    bool userExists(const string& username) {
        vector<User> users = FileManager::loadUsers();
        return any_of(users.begin(), users.end(), [&](const User& u) {
            return u.username == username;
        });
    }
};

class PostManager {
public:
    vector<Post> posts;

    PostManager() {
        posts = FileManager::loadPosts();
    }

    void createPost(const string& author) {
        Post newPost;
        cout << CYAN << "\n创建新帖子\n" << RESET;
        cout << "标题: ";
        getline(cin, newPost.title);
        cout << "内容: \n";
        getline(cin, newPost.content);

        time_t now = time(0);
        newPost.timestamp = ctime(&now);
        newPost.author = author;
        posts.push_back(newPost);

        FileManager::savePosts(posts);
        cout << GREEN << "帖子已发布！" << RESET << endl;
    }

    void displayPosts() {
        if (posts.empty()) {
            cout << YELLOW << "\n还没有帖子，快来第一个发言吧！" << RESET << endl;
            return;
        }

        cout << MAGENTA << "\n===== 最新帖子 =====\n" << RESET;
        for (const auto& post : posts) {
            cout << BLUE << "标题: " << RESET << post.title << endl
                 << CYAN << "作者: " << RESET << post.author
                 << " 时间: " << post.timestamp
                 << GREEN << "\n内容:\n" << RESET << post.content << endl;

            if (!post.comments.empty()) {
                cout << YELLOW << "\n评论:\n" << RESET;
                for (const auto& comment : post.comments) {
                    cout << "  • " << comment << endl;
                }
            }
            cout << MAGENTA << "-------------------\n" << RESET;
        }
    }

    void addComment(int postIndex, const string& author) {
        if (postIndex < 0 || postIndex >= posts.size()) return;

        cout << "输入评论内容: ";
        string comment;
        getline(cin, comment);
        posts[postIndex].comments.push_back(author + ": " + comment);
        FileManager::savePosts(posts);
        cout << GREEN << "评论已添加！" << RESET << endl;
    }
};

void displayWelcome() {
    system("cls || clear");
    cout << MAGENTA
         << "***************************************\n"
         << "*      欢迎来到 C++ 社交平台         *\n"
         << "***************************************\n"
         << RESET;
}

void mainMenu(PostManager& pm, AuthManager& auth) {
    while (true) {
        cout << CYAN << "\n主菜单\n"
             << "1. 查看帖子\n"
             << "2. 发布新帖\n"
             << "3. 添加评论\n"
             << "4. 退出\n"
             << "选择: " << RESET;

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            pm.displayPosts();
            break;
        case 2:
            pm.createPost(auth.currentUser.username);
            break;
        case 3:
            pm.displayPosts();
            cout << "输入要评论的帖子编号: ";
            int postIndex;
            cin >> postIndex;
            cin.ignore();
            pm.addComment(postIndex-1, auth.currentUser.username);
            break;
        case 4:
            return;
        default:
            cout << RED << "无效选项！" << RESET << endl;
        }
    }
}

int main() {
    AuthManager auth;
    PostManager pm;

    while (true) {
        displayWelcome();
        cout << "1. 登录\n"
             << "2. 注册\n"
             << "3. 退出\n"
             << "选择: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            if (auth.login()) {
                mainMenu(pm, auth);
            }
            break;
        case 2:
            auth.registerUser();
            break;
        case 3:
            return 0;
        default:
            cout << RED << "无效选项！" << RESET << endl;
        }
    }
}
