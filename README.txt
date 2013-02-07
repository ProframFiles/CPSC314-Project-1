Name: Aleksy Jones
Student #: 17798067
CS ID: q0b7

By submitting this file, I hereby declare that I worked individually on this assignment and that I am the only author of this code (except as noted). I have listed all external resoures (web pages, books) used below. I have listed all people with whom I have had significant discussions about the project below.


Sources:
All of the code was written by me, with two exceptions:

1) The stb image library (stb_image.h, stb_image.c), used to load in texture images. The API takes a filename, and provides pointer to image data. Public domain 'license' in the source files.

2) The simplex noise shader 'noise4D.glsl', used to help with flame and heat effects. The API is one function that takes a 4d vector and returns a float. MIT license, details in source file.

Otherwise I looked at stack overflow, wikipedia (for quaternion math), the opengl reference pages, lighthouse3d.com, the opengl wiki pages (very helpful for implementation details) and many misc forum postings. I may have C & P one or two lines from these places every now and then, but never more than that.

I started the project before the template was released using the iguana from a previous year, so some stuff might be different.

I got the animation method I used from the template for the the fall iteration of this course (glutTimerFunc)

Also the background cube map is from Humus 3D: http://www.humus.name/index.php?page=Textures
Creative commons license

The rest of the textures I made myself.

FILES:
#################
all the .png files are screenshots of the running program
.tga and .jpg pictures are textures that the program can use
.glsl files are SLSL shaders, generally split into vertex shaders and pixel/fragment shaders


USAGE:
################
Use the mouse to change the orientation of the scene
hold left button: rotate the rabbit around it's center of mass
hold right button: rotate the camera around the origin (keeping the rabbit in the same screen location)
hold middle button: translate the rabbit away and towards the camera

Extra functions
#################
'1' turns on FLAME EFFECTS!
'2' transitions to/from demon point cloud mode
'3' toggles non-stop animation (so we don't stop at the top of a jump)
'f' & 'v' adjusts the field of view up and down
'x' turns on automatic rotation and cycles through rotation axes
's' stops rotation
