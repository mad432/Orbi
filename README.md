# orbi

orbi is a n-body simulation built to simulate the evolution of various solar systems in 2d.

#

This repository is just a sample of some of the code I wrote.

the Particle file contains whats nessesary for each particle.
the System file (that I should probably rename) contains the functions for particle-particle interactions which is currently using cpu multithreading though I've been trying with limited success to incorperate Opencl and gpu processing on another branch though it is currently slower.
then the mainwindow file deals with the user interface.

#

Release.zip containbs the current windows deployment
