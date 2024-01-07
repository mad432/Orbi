# Orbi

orbi is a n-body simulation built to simulate the evolution of various solar systems in 2d.

#

the Particle file contains whats nessesary for each particle.
the System file (that I should probably rename) contains the functions for particle-particle interactions which is currently using cpu multithreading, though I've been trying with limited success to incorperate Opencl and gpu processing on another branch (that implementation is currently slower).
then the mainwindow file deals with the user interface. the Rocket file contains a flyable rocket that can be spawned. the Flight_plan file contains programs that can be used to have a rocket perform certain maneuvers

#

Release.zip contains the current windows deployment

## Usage

Clicking on the system view will cause a line to appear between where you click and the current mouse position( if the window size changes the positions might not lineup). clicking again will cause a particle to be created at the site of your initial click whose velocity will be determined by the previously mentioned line and whose mass will be determined by the slider in the bottom left. If fixed was checked when the particle was created the particle will stay still.
#
the G slider adjusts the force of gravity, and the time slider adjusts how large of a step each particle will move per tick (since the step size is adjusted slower speeds should be more accurate).
#
In the top left under file there are various prebuilt systems and controls for collisions.
next to file under the rocket tab lets you spawn rockets and contains pre-programed flights(messing with the system will break the program currently).
the Rocket can be controled using the W,A,D keys.
the relativity tab lets you enable special relativity in the program (the current implemetation of the rockets and collisions are not properly modeled when this setting is enabled)
