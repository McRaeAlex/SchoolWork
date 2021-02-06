# Assignment 3

Author: Alexander McRae

## Notes on compiling

Depending on the settings that are enabled with CMake you may have to turn on warnings as errors before compiling. I have had to do it before every lab and every assignment. It seems to be the provided code.

## Completed features

- Cube renders (2)
- Pinhole with perspective (1)
- Point light (1)
- Diffuse shading (2)

- Specular reflection (1)
- First person camera (2)

## How to view the features

Use wasd to move the camera around, Use your mouse to move the direction of the camera.

The cube defaults to spinning. This shows the specular reflection.

I suggest you move around the scene to view things best.

## Style

The code uses DSA functions.
The code uses RAII C++ style meaning the GPU data is freed when the object is no longer accessable and sent to the GPU when the object is created. This is C++ best practices [see here](http://www.stroustrup.com/bs_faq2.html#finally).

## Contact

If for some reason you need to contact me please call me at 905 746 1228 or email to mail@alexandermcrae.com
