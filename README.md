# Illustrate TCP self-connect

Due to two features (simultaneous open and ephemeral port range)
colliding, a process on Linux can make a TCP/IP connection to itself.
This can manifest as an [intermittant bug](https://goodenoughsoftware.net/2013/07/15/self-connects/).
[Decent explanations](http://sgros.blogspot.com/2013/08/tcp-client-self-connect.html) of
this exist.

The usual demo is a shell script like this:

    while true
    do
       telnet 127.0.0.1 50000
    done

I ran out of patience waiting for that to work, so I wrote a small
C program that usually connects to itself.

## Doing a TCP self-connect

In one `xterm`, build and execute the `self_connect` program:

    $ make
    $ make run
    ./self_connect localhost 50000
    worked after fail count 1936002
    my socket bound to 127.0.0.1:20675
    my peer bound to 127.0.0.1:20675
    Received "hello world!" from socket
    Pause, time for netstat -t, and ls -l /proc/1828/fd

Occasionally, `self_connect` gets a hit almost instantly. Other times,
you'll lose patience and ctrl-C it. When it does get a connection,
as above, try what it reccommends in another `xterm`:

	$ netstat -t
    Active Internet connections (w/o servers)
    Proto Recv-Q Send-Q Local Address           Foreign Address         State      
    ...
    tcp        0      0 192.168.0.106:38090     stackoverflow.com:https ESTABLISHED
    tcp        0      0 localhost.localdo:50000 localhost.localdo:50000 ESTABLISHED

	$ ls -l /proc/1828/fd
    total 0
    lrwx------ 1 bediger bediger 64 May 19 19:41 0 -> /dev/pts/0
    lrwx------ 1 bediger bediger 64 May 19 19:41 1 -> /dev/pts/0
    lrwx------ 1 bediger bediger 64 May 19 19:41 2 -> /dev/pts/0
    lrwx------ 1 bediger bediger 64 May 19 19:41 3 -> socket:[22872882]


It really does connect to itself on TCP port 50000. It sends a famous
phrase to iself to prove that the connection works.

I don't know why `getpeername()` and `getsockname()` both report
that they're using port 20675.
