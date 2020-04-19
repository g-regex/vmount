vmount - Mount multiple volumes using the same password with veracrypt

Usage: vmount CONTAINER1 MOUNTPOINT1 [CONTAINER2 MOUNTPOINT2] ...

This is just scratch and not recommended to use for anyone :D

If you use it anyway, make sure you have the right permissions to mount volumes with veracrypt. Tested with veracrypt-1.24.

vmount currently assumes that your PIM is 123.

TODO:
-almost everything

-input checking

-checks whether volumes exist

-checks whether volumes are already mounted

-version checking

-multiple version support

-...
