// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#define PORT 8080
#include <thread>   

using namespace std;

bool Working=true;
string usertitle = "";

void thread_Send(int sock)
{
    printf("Please enter a line:\n");
    char *line = NULL;
    size_t len = 0;
    ssize_t lineSize = 0;
    while (Working)
    {
        lineSize = getline(&line, &len, stdin);
        string sline(line);

        if(sline.at(0) == '/' && sline.at(1) == 'e') {
            Working = false;
            cout<<"Exiting"<<endl;
            exit(1);
        }

        if(sline.at(0) == '/' && sline.at(1) == 'f') {
            string filename = sline.substr(3);
            while (filename[filename.length()-1] == '\n') {
                filename = filename.substr(0, filename.length()-1);
            }
            std::ifstream inputA(filename);
            if (inputA.good()) {
                string a;
                string contents;

                while(getline(inputA, a)) {
                    contents += a + '\n';
                }
                
                inputA.close();

                contents = usertitle + " : /f " + filename + " " + contents + "#";
                char *fileStuff = new char [contents.length() + 1];
                strcpy(fileStuff, contents.c_str());
                lineSize = contents.length();

                send(sock , fileStuff , lineSize , 0 );

            } else {
                cout << "File Not Found"<<endl;
            }

            sline = "";
        } else if(sline.at(0) == '/' && sline.at(1) == 'u') {
            string fileInfo = sline.substr(3);
            string userselect = fileInfo.substr(0, fileInfo.find(' '));
            string fileselect = fileInfo.substr(fileInfo.find(' ') + 1);
            
            
            while (fileselect[fileselect.length()-1] == '\n') {
                fileselect = fileselect.substr(0, fileselect.length()-1);
            }

            std::ifstream inputB(fileselect);
            if (inputB.good()) {
                string b;
                string contentsB;

                while(getline(inputB, b)) {
                    contentsB += b + '\n';
                }
                
                inputB.close();

                contentsB = usertitle + " : /u " + userselect + " " + fileselect + " " + contentsB + "#";
                char *fileStuffB = new char [contentsB.length() + 1];
                strcpy(fileStuffB, contentsB.c_str());
                lineSize = contentsB.length();

                send(sock , fileStuffB , lineSize , 0 );

            } else {
                cout << "File Not Found"<<endl;
            }

            sline = "";
        }

        while (sline[sline.length()-1] == '\n') {
            sline = sline.substr(0, sline.length()-1);
        }

        if (sline.length() != 0) {
            sline = usertitle + " : " + sline + "#";

            char *newline = new char [sline.length() + 1];
            strcpy(newline, sline.c_str());
            lineSize = sline.length();

            send(sock , newline , lineSize , 0 );
        }
    }
    
    
}

void thread_Receive(int Sock)
{
    char buffer[1024] = {0};
    int  valread;
    while (Working)
    {
        valread = read( Sock , buffer, 1024);
        if (valread>0)
        {
            string reline(buffer);
            reline = reline.substr(0, reline.find('#'));
            
            char command = 'a';
            if (reline.length() - reline.find(':') >= 5) {
                command = reline.at(reline.find(':') + 3);
            }

            if ((reline.at(reline.find(':') + 2) == '/') && (command == 'f')) {
                string recontents = reline.substr(reline.find(':') + 5);
                string refilename = recontents.substr(0, recontents.find(' '));
                recontents = recontents.substr(refilename.length() + 1);

                ofstream outputA(refilename);
                
                if (outputA) { 
                    outputA << recontents;
                }
                outputA.close();
                cout<<reline.substr(0, reline.find(' '))<<" sent file called "<< refilename <<endl; 
                
            } else if ((reline.at(reline.find(':') + 2) == '/') && (command == 'p')) {
                string reuser = reline.substr(reline.find('/')+3);
                reuser = reuser.substr(0, reuser.find(' '));
                if (reuser.compare(usertitle) == 0) {
                    string sender = reline.substr(0, reline.find(' '));
                    reline = reline.substr(usertitle.length() + sender.length() + 6);
                    reline = sender + " : " + reline;
                    cout<<reline<<" (PM)"<<endl;
                } 
            } else if ((reline.at(reline.find(':') + 2) == '/') && (command == 'u')) {
                
                string sender = reline.substr(0, reline.find(' '));
                string nameFileText = reline.substr(reline.find('/')+3);
                string reciever = nameFileText.substr(0, reline.find(' ')+1);
                if (reciever.compare(usertitle) == 0) {
                    string fileoutputText = nameFileText.substr(reciever.length()+1);
                    string fileoutput = fileoutputText.substr(0, fileoutputText.find(' '));
                    string messageText = fileoutputText.substr(fileoutput.length() + 1);

                    ofstream outputB(fileoutput);
                
                    if (outputB) { 
                        outputB << messageText;
                    }
                    outputB.close();
                    cout<<sender<<" sent file called "<< fileoutput <<" (PM)"<<endl; 
                }
            } else {
                cout<<reline<<endl;
            }
        }
        
    }
    
}

bool checkLogin(string username, string password, int sock) {
    string loginStatement = "/login-" + username + "_" +  password + "#";
    char *line = new char [loginStatement.length() + 1];
    strcpy(line, loginStatement.c_str());
    ssize_t lineSize = 0;
    lineSize = loginStatement.length();
    send(sock , line , lineSize , 0 );
    
    char buffer[1024] = {0};
    int  valread;
    
    valread = read( sock , buffer, 1024);

    memset(line, 0, loginStatement.length() * (sizeof line[0]));
    loginStatement = "";

    if(buffer[0] == 't') {
        usertitle = username;
        return true;
    } else {
        return false;
    }
}

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    const char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    

    bool login = false;

    while(login == false) {
        string username = "";
        string password = "";
        cout << "Please Enter Username: ";
        cin >> username;
        cout << "Please Enter Password: ";
        cin >> password;
        login = checkLogin(username, password, sock);
    }

    thread Thread_send_Obj(thread_Send,sock);
    thread Thread_Receive_Obj(thread_Receive,sock);
    
    Thread_send_Obj.join();
    Thread_Receive_Obj.join();
    
    return 0;
}
