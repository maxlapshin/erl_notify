Erlang poll limitations and sysfs_notify
========================================


This repository is illustrating small imperfection in erlang polling mechanism:

  erlang cannot poll reads on regular files.


Linux kernel has sysfs_notify mechanism. Process that is waiting for change in 
some kernel value should open "regular" file in /sys filesystem and start polling on it:

```
loop {
  poll(fd)
  seek(fd,0)
  read(fd)
}
```

Kernel driver can trigger this poll event by calling sysfs_notify.
This sysfs_notify call will take a list of all polling subscribers and notify them
about change in the descriptor.

So it is not "new data arrived" but "something has changed, recheck it".

All reads from /sys filesystem are made in non-blocking mode.

This polling functionality is required for any modern access to hardware that is made with event-based
approach.

How to check?
-------------

I've prepared three files for you: kernel driver source, userspace working C program and non-working erlang program.

* install `apt install build-essential linux-headers-$(uname -r)`
* run `make` to compile everything 
* now `insmod erl_notify.ko`
* `./poller`

After launching poller you will see how one process (one thread) is changing something in kernel by writing to /sys
and second thread is polling on the read descriptor to wait for changes.

Then launch `./poll.erl`.  You will see endless fast reads that are made in non-blocking mode.

Let's change it!



