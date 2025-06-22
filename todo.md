# General TODO

- [ ] Official build system (**better build system**) Makefile
- [x] Send a HTTP POST request using libcurl to gain an auth token 
- [x] Figure out if I want to use IRC or EventSubs provided by Twitch API (IRC)
- [x] Test IRC manually with telnet irc.chat.twitch.tv 6667 
    * Getting a NOTICE * :Login unsuccessful (Didn't have chat read/write access)
- [ ] Create a simple IRC Client to connect to irc.chat.twitch.tv
    -  [x] Create a TCP/IP client to connect to irc.chat.twitch.tv on port 6667 (Non-SSL)
    -  [x] Authenticate the 'TestingTestingZai' bot
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

### 6/19/2025
- [x] Figure out where I last left off
- [ ] Create a system for testing to generate a new auth token so I don't have to keep manually generating one

### 6/21/2025
- [ ] Create a message based off the trailing params

### 6/22/2025
- [x] Plan out the logic and data flow on how consistently receive each
message from my twitch chat
- [x] Send over the JOIN message 
- [ ] Handle PING message from Twitch

### Case 1
:tmi.twitch.tv [prefix] 0
421 [command] 1
you [param] 2
QPMLCJ8FHSTBLNME [param] 3

:Unknown [trailing param] 4
command/r/n [trailing param] 5 



