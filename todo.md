# General TODO

- [ ] Official build system (**better build system**) Makefile
- [x] Send a HTTP POST request using libcurl to gain an auth token 
- [x] Figure out if I want to use IRC or EventSubs provided by Twitch API (IRC)
- [x] Test IRC manually with telnet irc.chat.twitch.tv 6667 
    * Getting a NOTICE * :Login unsuccessful (Didn't have chat read/write access)
- [ ] Create a simple IRC Client to connect to irc.chat.twitch.tv
    -  [x] Create a TCP/IP client to connect to irc.chat.twitch.tv on port 6667 (Non-SSL)
    -  [ ] Authenticate the 'TestingTestingZai' bot
    -  [ ] Handle twitch response messages
        - [ ] Handle the 'PING' command so that I could respond with 'PONG' to keep the connection alive (Listen for this message)


## ZTwitch-Client-Chat Flow

The goal is to have an application that simply connect any twitch channel.

z-twitch-client <twitch_channel_name>

- User will need a auth token
- login using the users auth token (require the user to store their auth token as an environment variable) if no auth token, do not connect 
- connect to twitch irc server irc.chat.twitch.tv 6667
- As the client we will send the following messages
    - PASS oauth:<access token>
    - JOIN #<twitch_channel_name>
- Listen for every twitch chat message
- Monitor the connection (Implement some sort of Listening for messages system)
- Implement an option to have the ability to send messages to a twitch chat

## Questions/Understanding

- How do I handle a twitch chat that is moving at a fast rate?
- Per the IRC spec the max length of a message is 512 bytes. Twitch has set the max character count to 500 bytes.

## Issues

### 6/11/2025

- Cannot connect to Twitch [FIXED]
- Need to use getaddrinfo to construct my ip address [FIXED] used inet_pton
- and figure out as to what I'm doing wrong when setting the server address [Needed to used the actual IP of the IRC server not the string version]


### 6/14/2025

- I want to add Terminal UI stuff such as a loading bar
- Check if my authentication messages have been sent. 
- How do I know if my messages get sent? Well I guess it depends on the type of message?



{0} :
{1} t
{2} m
{3} i
{4} .
{5} t
{6} w
{7} i
{8} t
{9} c
{10} h
{11} .
{12} t
{13} v
{14}  
{15} 4
{16} 2
{17} 1
{18}  
{19} y
{20} o
{21} u
{22}  
{23} Q
{24} P
{25} M
{26} L
{27} C
{28} J
{29} 8
{30} F
{31} H
{32} S
{33} T
{34} B
{35} L
{36} N
{37} M
{38} E
{39}  
{40} :
{41} U
{42} n
{43} k
{44} n
{45} o
{46} w
{47} n
{48}  
{49} c
{50} o
{51} m
{52} m
{53} a
{54} n
{55} \r
{56} \n

