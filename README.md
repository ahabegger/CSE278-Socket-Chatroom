# CSE 278 Project

Team Members : Alex Habegger (habeggaj) and Nathan Abdoo (abdoojm)

Contact Email : habeggaj@miamioh.edu, abdoojm@miamioh.edu


**What is the project you plan to do?**

Chatting Room


**What are the functionalities you need to implement in this project? (I.e., explain what your application can do or what we can do with your application)**

Login check
Chatting freely - Type anytime, receive messages instantly
	Send messages to all users
	Send message privately to one user
Send files
Exit client (without Ctrl + C)

**What data is needed in your project? How will you store the data? How many tables (attributes, foreign key) do you need?**

LoginInfo - Username and Password columns
	One Table in SQL

**List all the files you need to implement and write down the functionalities of each file you want to create. List all the functions you need to implement to complete the project.**

_Server.cpp_
sendTo (message, listUsers) 
Overload sendTo (file, listUsers) 
receive ()

_Client.cpp_
login (username, password) 
sendAll (message) 
Overload sendAll (file)
privateMessage (username, message)
Overload privateMessage (username, file)
receive ()
exit ()

