# You Spin Me Round Robin

The round-robin scheduler, implemented in C, uses a TAILQ queue to manage processes, adding fields to the process struct to track remaining time, first run time, completion time, and start status. It executes processes for the minimum of quantum length or remaining time, calculating waiting time (completion time minus arrival and burst times) and response time (first run time minus arrival time) until all processes complete.

## Building

```shell
make
```

## Running

cmd for running TODO
```shell
./rr cmd, for example:
./rr processes.txt 3
```

results TODO
```shell
Average waiting time: 7.00
Average response time: 2.75
```

## Cleaning up

```shell
make clean
```
