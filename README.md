# Illustrate TCP self-connect

http://sgros.blogspot.com/2013/08/tcp-client-self-connect.html


## Doing a TCP self-connect

    $ make
    $ make run
    ./self_connect localhost 50000
    worked after fail count 1936002
    my socket bound to 127.0.0.1:20675
    my peer bound to 127.0.0.1:20675
    Received "hello world!" from socket
    Pause, time for netstat -t, and ls -l /proc/1828/fd

Meanwhile, in another `xterm`:

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

