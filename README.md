# tcpcontroller
Well, this is simple app, made just for fun that enables a possibility to remote control computer using simple commands.
You can create your own commands using a "config" file (or even multiple files) placed somewhere on the computer. Besides that
you can also set a "password" for client to authenticate.

# How does it work (files)
It creates a lambda expression and stores it in the map, so if user sends the command, it does sort of "switch case" things
and choses the right lambda expression or previously defined functions (commands.h).

# Usage
"NSQL Server.exe" { Location of config folder e.g. C:\config } 

# TODO
- GUI (gui branch, flutter)
- comments for sure
- client (for now I'm using an app called TCP Client for android)
