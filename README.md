# CPSC 314 project #1: An Animated Rabbit

A rabbit made only out of affine-transformed unit cubes. Features cube mapped reflections and screen-space flame effects.

## BUILDING:

on linux, 'make' should just work
on windows, use the provided .sln file 

That said, I've tested this on a total of two systems, so YMMV.

## USAGE:

See the assignment description in P1.pdf for the list of supported commands

#### Additional commands

* Use the mouse to change the orientation of the scene
* hold left button: rotate the rabbit around it's center of mass
* hold right button: rotate the camera around the origin (keeping the rabbit in the same screen location)
* hold middle button: translate the rabbit away and towards the camera
* '1' turns on FLAME EFFECTS!
* '2' transitions to/from demon point cloud mode
* '3' toggles non-stop animation (so we don't stop at the top of a jump)
* 'f' & 'v' adjusts the field of view up and down
* 'x' turns on automatic rotation and cycles through rotation axes
* 's' stops rotation
