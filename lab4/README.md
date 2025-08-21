# Hey! I'm Filing Here

In this lab, I successfully implemented a 1 MiB ext2 file system with 2 directories, 1 regular file, and 1 symbolic link.

## Building
```make``` will compile the executable

```./ext2-create``` will create our file system image cs111-base.img

## Running
We can dump the file system information using  ```dumpe2fs cs111-base.img```

We can run ```fsk.ext2 cs111-base.img``` to check through our file system.

We can also mount our file system using ```sudo mount -o loop cs111-base.img mnt``` and then run ```ls -ain mnt/```

## Cleaning up

Unmount the file system using the command  ```sudo umount mnt``` and remove the folder using ```rmdir mnt```

Then clean other files using ```make clean```
