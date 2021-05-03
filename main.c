//
// Created by tinykishore on 5/3/21.
//

/**
  The Following Code uses Preprocessor Directives and File Operations. This code makes a file (test.txt) in Desktop
  for both Windows and Linux machine. If it is compiled on a linux machine, the file will be created at

       /home/$username$/Desktop/

  location, and if compiled on a Windows machine, the code will first try to open test file in

       C:\\Users\$USERNAME$\OneDrive\Desktop\

  This is the location for Windows 10 if OnDrive Feature is enabled. If Failed, it again tries to open test file in

       C:\\Users\$USERNAME$\Desktop\

  This is the location for Windows 7 and earlier, Windows 10 with OneDrive feature disabled. If File Operations fails
  in every cases, The program will stop, exiting with ERROR CODE: EXIT_FAILURE.

 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>

const char *UNIXGetUserName();

char *extractUsername(const char *, char *);

int main(__attribute__((unused))int argc, __attribute__((unused))char const *argv[]) {
    FILE *f;

#if __linux__ /////////// LINUX CODE /////////////////

#ifndef PATH
#define PATH "/Desktop/test.txt"
#endif

    // Creating PATH
    char path[100] = "/home/";
    char *username;

    // Assigning Username
    if (UNIXGetUserName() == NULL) {
        printf("Cannot retrieve Username\n");
        exit(EXIT_FAILURE);
    } else username = UNIXGetUserName();
    strcat(path, strcat(username, PATH));

    // File Operation
    f = fopen(path, "w+");
    if (f) {
        printf("test.txt CREATED AT: ");
        printf("%s\n", path);
    } else {
        printf("ERROR OPENING FILE");
        exit(EXIT_FAILURE);
    }

#elif WIN32 /////////// WINDOWS CODE /////////////////

    #ifndef PATH
#define PATH "\\OneDrive\\Desktop\\test.txt"
#endif

#ifndef LEGACY_PATH
#define LEGACY_PATH "\\Desktop\\test.txt"
#endif

    // Creating PATH
    char path[100] = "C:\\Users\\";
    char username[50];
    extractUsername(getenv("USERPROFILE"), username);
    strcat(path, strcat(username, PATH));

    // Creating LEGACY_PATH
    char legacy_path[100] = "C:\\Users\\";
    char legacy_username[50];
    extractUsername(getenv("USERPROFILE"), legacy_username);
    strcat(legacy_path, strcat(legacy_username, LEGACY_PATH));

    // File Operation
    f = fopen(path, "w+");
    if (!f) {
        f = fopen(legacy_path, "w+");
        if(!f){
            printf("ERROR OPENING FILE");
            exit(EXIT_FAILURE);
        } else{
            printf("test.txt CREATED AT: ");
            printf("%s\n", legacy_path);
        }
    } else {
        printf("test.txt CREATED AT: ");
        printf("%s\n", path);
    }

#endif

    fclose(f);
    return 0;
}

char *extractUsername(const char *username, char *usr) {
    int length_of_username = 0, index_of_slash = 0, index = 0;
    for (int i = 0; username[i] != '\0'; ++i) length_of_username++;
    for (int i = length_of_username - 1; i >= 0; --i) {
        if (username[i] == '\\') {
            index_of_slash = i;
            break;
        } else continue;
    }
    for (int i = index_of_slash + 1; i < length_of_username; ++i, ++index) {
        usr[index] = username[i];
    }
    usr[index] = '\0';
    return usr;
}

const char *UNIXGetUserName() {
    uid_t id = geteuid();
    struct passwd *usr = getpwuid(id);
    if (usr) return usr->pw_name;
    return NULL;
}