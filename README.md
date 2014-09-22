LVA
===

_LVA = Lunettes à Vision Augmentée (~Augmented Vision Glasses)_

School project, second year.

Augmented Vision
----------------

Provide a way for partially blind people (but sensitive to light) to walk around
in a closed environment.

Components
----------

* [BeagleBoneBlack](http://beagleboard.org/black), _à la_ raspberry pi board
* [Xtion Pro](http://www.asus.com/Multimedia/Xtion_PRO/), depth sensor (~_kinect_)
* [OpenNI](https://github.com/OpenNI/OpenNI), read data from the depth camera
* [ArchlinuARM](http://archlinuxarm.org/), linux distribution

DEMO
----

![img](https://raw.githubusercontent.com/jeanbroid/LVA/master/demo.gif)

Youtube channel [link](https://www.youtube.com/playlist?list=PLQOvkFFH_MHpGyxOkTCT55TL4DnkGuFKl)

Todo:
-----

* ~~independant pin gpios setup~~
* ~~rts~~
* ~~signal (SIGINT)~~
* ~~threads (playing oni)~~
* ~~pthread_join~~
* Makefile (debug/ release)

Help:
-----
* http://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
