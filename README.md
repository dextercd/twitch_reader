# twitch_reader


C++ ASIO Program that can go into an IRC or a twitch chat and read it's messages! It parses the messages and turns them into an easy irc interface. It includes code to change the irc::message type to a twitch::message, which gives easy control to query the display_name, bits count, subscription months count, and more!

It uses C++17 \<iterator>'s std::size() for array size, but other than that a C++14 compiler should work.

I know it runs on both Linux and Windows, I see no reason why it wouldn't run on other OS's.

To run you need to have a file named user_data.txt which has the reader's username on the first line and the oauth token on the next line, then start if via a command prompt like this: `program bobross` the program will join bob's channel with the info in user_data.txt 


*NOTE*: I'm learning CMake, IRC, and git+Github as I go, if you see something you think I'm doing wrong, please tell me!
